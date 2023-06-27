#include <benchmark.h>
#include "cvpinst.h"
#include "utils.h"

#define N_SIZE 33

static uint64_t *A1, *A2, *B, *C;
static int test_pass;
static int elem_bits[4] = {2, 4, 8, 16};
static int elem_width[4] = {0x3, 0xf, 0xff, 0xffff};
static int elem_min[4] = {-2, -8, -128, -32768};
static int elem_max[4] = {1, 7, 127, 32767};

void bench_add_prepare() {
  bench_srand(1);
  test_pass = 1;
}

void bench_add_run() {
  int pass1, pass2;
  int reg_num, elem_per_reg;
  uint8_t sew;

  for(sew=0; sew<=3; sew++) {
    reg_num = RoundUpDiv(N_SIZE * N_SIZE * elem_bits[sew], 64);
    elem_per_reg = 64 / elem_bits[sew];
    printf("\nsew=%d begin: reg_num=%d, elem_per_reg=%d, out_size=%d\n", sew, reg_num, elem_per_reg, N_SIZE);

    A1 = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_num);
    A2 = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_num);
    B = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_num);
    C = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_num);

    printf(" init A1 ...\n");
    for (int i=0; i < N_SIZE * N_SIZE; i++) {
      int16_t rand = bench_rand();
      int16_t temp = (bench_rand() % 2) ? -rand : rand;
      PutRawData(A1, i, sew, temp);
      if (i % 100 == 0)
        printf("  %d/%d \t", i, N_SIZE * N_SIZE);
    }
    printf("\n init A2 ...\n");
    for (int i=0; i < N_SIZE * N_SIZE; i++) {
      int16_t rand = bench_rand();
      int16_t temp = (bench_rand() % 2) ? -rand : rand;
      PutRawData(A2, i, sew, temp);
      if (i % 100 == 0)
        printf("  %d/%d \t", i, N_SIZE * N_SIZE);
    }

    int zp;
    uint64_t zp_reg;
    if (sew == 0) zp = rand() % 4 - 2;
    else if (sew == 1) zp = rand() % 16 - 8;
    else if (sew == 2) zp = rand() % 256 - 128;
    else zp = rand() % 65536 - 32768;
    zp_reg = zp & 0xffff;
    printf("\n zp: %d, zp_reg: %#lx\n", zp, zp_reg);

    uint64_t temp, temp_std;

    // add.n
    printf(" add.n begin ...\n");
    pass1 = 1;
    for (int i=0; i < reg_num; i++) {
      temp = Add_N(A1[i], A2[i], zp_reg, sew);
      B[i] = temp;

      temp_std = 0;
      for (int j=0; j < elem_per_reg; j++) {
        int tmp_sum = GetRawData(A1, i * elem_per_reg + j, sew) + GetRawData(A2, i * elem_per_reg + j, sew) - zp;
        tmp_sum = min(max(tmp_sum, elem_min[sew]), elem_max[sew]);
        temp_std |= (uint64_t)(tmp_sum & elem_width[sew]) << (j * elem_bits[sew]);
      }
      C[i] = temp_std;

      if (B[i] != C[i]) {
        printf("  add.n error: i=%d, add_res=%#lx, std_res=%#lx\n", i, temp, temp_std);
        for (int si=0; si < elem_per_reg; si++) {
          printf("  %d,", GetRawData(A1, i * elem_per_reg + si, sew));
        }
        printf("\n");
        for (int si=0; si < elem_per_reg; si++) {
          printf("  %d,", GetRawData(A2, i * elem_per_reg + si, sew));
        }
        printf("\n");
        pass1 = 0;
      }
      else {
        ;//printf("  ok: i=%d, tmp_res=%d\n", i, temp);
      }
    }

    // add.relu6
    printf(" add.relu6 begin ...\n");
    pass2 = 1;
    for (int i=0; i < reg_num; i++) {
      temp = Add_Relu(A1[i], A2[i], zp_reg, sew);
      B[i] = temp;

      temp_std = 0;
      for (int j=0; j < elem_per_reg; j++) {
        int tmp_sum = GetRawData(A1, i * elem_per_reg + j, sew) + GetRawData(A2, i * elem_per_reg + j, sew) - zp;
        tmp_sum = min(max(tmp_sum, zp), elem_max[sew]);
        temp_std |= (uint64_t)(tmp_sum & elem_width[sew]) << (j * elem_bits[sew]);
      }
      C[i] = temp_std;

      if (B[i] != C[i]) {
        printf("  add.relu error: i=%d, add_res=%#lx, std_res=%#lx\n", i, temp, temp_std);
        for (int si=0; si < elem_per_reg; si++) {
          printf("  %d,", GetRawData(A1, i * elem_per_reg + si, sew));
        }
        printf("\n");
        for (int si=0; si < elem_per_reg; si++) {
          printf("  %d,", GetRawData(A2, i * elem_per_reg + si, sew));
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
    bench_free(A1);
    bench_free(A2);
    bench_free(B);
    bench_free(C);
  }
}

int bench_add_validate() {
  return (setting->checksum == 0x00000031) && test_pass;
}
