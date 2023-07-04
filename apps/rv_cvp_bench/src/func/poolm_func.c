#include <benchmark.h>
#include "cvpinst.h"
#include "utils.h"

#define N_SIZE 20
#define KERNEL_SIZE 2
#define STRIDE 2

static uint64_t *Am, *Aa;
static int32_t *Bm, *Ba, *Cm, *Ca;
static int test_pass;
static int elem_bits[4] = {2, 4, 8, 16};
static int elem_width[4] = {0x3, 0xf, 0xff, 0xffff};
static int elem_min[4] = {-2, -8, -128, -32768};

void bench_poolm_prepare() {
  bench_srand(1);
  test_pass = 1;
}

void bench_poolm_run() {
  int pass1, pass2;
  int reg_num, elem_per_reg, elem_num;
  int out_size;
  uint8_t sew;

  for(sew=0; sew<=3; sew++) {
    out_size = (N_SIZE - KERNEL_SIZE) / STRIDE + 1;
    reg_num = RoundUpDiv(KERNEL_SIZE * KERNEL_SIZE * out_size * out_size * elem_bits[sew], 64);
    elem_per_reg = 64 / elem_bits[sew];
    elem_num = KERNEL_SIZE * KERNEL_SIZE * out_size * out_size;
    printf("\nsew=%d begin: reg_num=%d, elem_per_reg=%d, elem_num=%d, out_size=%d\n", sew, reg_num, elem_per_reg, elem_num, out_size);

    Am = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_num);
    Aa = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_num);
    Bm = (int32_t *)bench_alloc(sizeof(int16_t) * reg_num);
    Ba = (int32_t *)bench_alloc(sizeof(int16_t) * reg_num);
    Cm = (int32_t *)bench_alloc(sizeof(int16_t) * reg_num);
    Ca = (int32_t *)bench_alloc(sizeof(int16_t) * reg_num);

    printf(" init A ...\n");
    for (int i=0; i < reg_num * elem_per_reg; i++) {
      int16_t rand = bench_rand();
      int16_t temp = (bench_rand() % 2) ? -rand : rand;
      if (i < elem_num) {
        PutRawData(Am, i, sew, temp);
        PutRawData(Aa, i, sew, temp);
      }
      else {
        PutRawData(Am, i, sew, elem_min[sew]);
        PutRawData(Aa, i, sew, 0);
      }
      if (i % 50 == 0)
        printf("  %d/%d \t", i, reg_num * elem_per_reg);
    }

    int16_t temp, temp_std;

    // pool.m.max
    printf(" pool.m.max begin ...\n");
    pass1 = 1;
    for (int i=0; i < reg_num; i++) {
      temp = Pool_M_Max(Am[i], sew);
      //printf("   i=%d: tmp=%d ", i, temp);
      Bm[i] = temp;

      temp_std = 0;
      for (int j=0; j < elem_per_reg / 4; j++) {
        int16_t temp_max = elem_min[sew];
        for (int k=0; k < 4; k++) {
          temp_max = max(temp_max, GetRawData(&Am[i], j * 4 + k, sew));
        }
        temp_std |= (temp_max & elem_width[sew]) << (j * elem_bits[sew]);
      }
      Cm[i] = temp_std;

      if (Bm[i] != Cm[i]) {
        printf("  pool.m.max error: i=%d, poolmmax_res=%d, std_res=%d\n", i, temp, temp_std);
        for (int si=0; si < elem_per_reg; si++) {
          printf("  %d,", GetRawData(&Am[i], si, sew));
        }
        printf("\n");
        pass1 = 0;
      }
      else {
        ;//printf("  ok: i=%d, tmp_res=%d\n", i, temp);
      }
    }

    // pool.m.avg
    printf(" pool.m.avg begin ...\n");
    pass2 = 1;
    for (int i=0; i < reg_num; i++) {
      temp = Pool_M_Avg(Aa[i], sew);
      //printf("   i=%d: tmp=%d ", i, temp);
      Ba[i] = temp;

      temp_std = 0;
      for (int j=0; j < elem_per_reg / 4; j++) {
        int temp_avg = 0;
        for (int k=0; k < 4; k++) {
          temp_avg += GetRawData(&Aa[i], j * 4 + k, sew);
        }
        temp_std |= ((temp_avg / 4) & elem_width[sew]) << (j * elem_bits[sew]);
      }
      Ca[i] = temp_std;

      if (Ba[i] != Ca[i]) {
        printf("  pool.m.avg error: i=%d, poolmavg_res=%d, std_res=%d\n", i, temp, temp_std);
        for (int si=0; si < elem_per_reg; si++) {
          printf("  %d,", GetRawData(&Aa[i], si, sew));
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
    bench_free(Am);
    bench_free(Aa);
    bench_free(Bm);
    bench_free(Ba);
    bench_free(Cm);
    bench_free(Ca);
  }
}

int bench_poolm_validate() {
  return (setting->checksum == 0x00000041) && test_pass;
}
