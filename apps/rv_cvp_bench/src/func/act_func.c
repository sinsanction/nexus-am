#include <benchmark.h>
#include "cvpinst.h"
#include "utils.h"

#define N_SIZE 28

static uint64_t *A, *B, *C;
static int test_pass;
static int elem_bits[4] = {2, 4, 8, 16};
static int elem_width[4] = {0x3, 0xf, 0xff, 0xffff};

void bench_act_prepare() {
  bench_srand(1);
  test_pass = 1;
}

void bench_act_run() {
  int pass1, pass2;
  int reg_num, elem_per_reg;
  uint8_t sew;

  for(sew=0; sew<=3; sew++) {
    reg_num = RoundUpDiv(N_SIZE * N_SIZE * elem_bits[sew], 64);
    elem_per_reg = 64 / elem_bits[sew];
    printf("\nsew=%d begin: reg_num=%d, elem_per_reg=%d, out_size=%d\n", sew, reg_num, elem_per_reg, N_SIZE);

    A = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_num);
    B = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_num);
    C = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_num);

    printf(" init A ...\n");
    for (int i=0; i < N_SIZE * N_SIZE; i++) {
      int16_t rand = bench_rand();
      int16_t temp = (bench_rand() % 2) ? -rand : rand;
      PutRawData(A, i, sew, temp);
      if (i % 100 == 0)
        printf("  %d/%d \t", i, N_SIZE * N_SIZE);
    }

    int zp, q6;
    uint64_t zp_reg;
    if (sew == 0) { q6 = bench_rand() % 4; zp = bench_rand() % (q6 + 1); q6 -= 2; zp -= 2; }
    else if (sew == 1) { q6 = bench_rand() % 16 ; zp = bench_rand() % (q6 + 1); q6 -= 8; zp -= 8; }
    else if (sew == 2) { q6 = bench_rand() % 256; zp = bench_rand() % (q6 + 1); q6 -= 128; zp -= 128; }
    else { q6 = bench_rand() % 65536; zp = bench_rand() % (q6 + 1); q6 -= 32768; zp -= 32768; }
    zp_reg = ((q6 & 0xffff) << 16) | (zp & 0xffff);
    printf("\n zp: %d, q6: %d, zp_reg: %#lx\n", zp, q6, zp_reg);

    uint64_t temp, temp_std;

    // act.relu
    printf(" act.relu begin ...\n");
    pass1 = 1;
    for (int i=0; i < reg_num; i++) {
      temp = Act_Relu(A[i], zp_reg, sew);
      B[i] = temp;

      temp_std = 0;
      for (int j=0; j < elem_per_reg; j++) {
        temp_std |= ((uint64_t)max(GetRawData(A, i * elem_per_reg + j, sew), zp) & elem_width[sew]) << (j * elem_bits[sew]);
      }
      C[i] = temp_std;

      if (B[i] != C[i]) {
        printf("  act.relu error: i=%d, act_res=%#lx, std_res=%#lx\n", i, temp, temp_std);
        for (int si=0; si < elem_per_reg; si++) {
          printf("  %d,", GetRawData(A, i * elem_per_reg + si, sew));
        }
        printf("\n");
        pass1 = 0;
      }
      else {
        ;//printf("  ok: i=%d, tmp_res=%d\n", i, temp);
      }
    }

    // act.relu6
    printf(" act.relu6 begin ...\n");
    pass2 = 1;
    for (int i=0; i < reg_num; i++) {
      temp = Act_Relu6(A[i], zp_reg, sew);
      B[i] = temp;

      temp_std = 0;
      for (int j=0; j < elem_per_reg; j++) {
        temp_std |= ((uint64_t)min(max(GetRawData(C, i * elem_per_reg + j, sew), zp), q6) & elem_width[sew]) << (j * elem_bits[sew]);
      }
      C[i] = temp_std;

      if (B[i] != C[i]) {
        printf("  act.relu error: i=%d, act_res=%#lx, std_res=%#lx\n", i, temp, temp_std);
        for (int si=0; si < elem_per_reg; si++) {
          printf("  %d,", GetRawData(A, i * elem_per_reg + si, sew));
        }
        printf("\n");
        pass2 = 0;
      }
      else {
        ;//printf("  ok: i=%d, tmp_res=%d\n", i, temp);
      }
    }

    if (pass1 == 1 && pass2 == 1) {
      printf("sew=%d end: pass!!!\n", sew);
    }
    else {
      printf("sew=%d end: fail %d, %d\n", sew, pass1, pass2);
      test_pass = 0;
    }
    bench_free(A);
    bench_free(B);
    bench_free(C);
  }
}

int bench_act_validate() {
  return (setting->checksum == 0x00000021) && test_pass;
}
