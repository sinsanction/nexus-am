#include <benchmark.h>
#include "cvpinst.h"
#include "utils.h"

#define N_SIZE 21
#define KERNEL_SIZE 3
#define STRIDE 3

static uint64_t *Am, *Aa;
static int32_t *Bm, *Ba, *Cm, *Ca;
static int test_pass;
static int elem_bits[4] = {2, 4, 8, 16};
static int elem_min[4] = {-2, -8, -128, -32768};

void bench_pool_prepare() {
  bench_srand(1);
  test_pass = 1;
}

void bench_pool_run() {
  int pass1, pass2;
  int reg_per_line, elem_per_reg, elem_per_line;
  int out_size;
  uint8_t sew;

  for(sew=0; sew<=3; sew++) {
    out_size = (N_SIZE - KERNEL_SIZE) / STRIDE + 1;
    reg_per_line = RoundUpDiv(KERNEL_SIZE * KERNEL_SIZE * elem_bits[sew], 64);
    elem_per_reg = 64 / elem_bits[sew];
    elem_per_line = KERNEL_SIZE * KERNEL_SIZE;
    printf("\nsew=%d begin: reg_per_line=%d, elem_per_reg=%d, elem_per_line=%d, out_size=%d\n", sew, reg_per_line, elem_per_reg, elem_per_line, out_size);

    Am = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_per_line * out_size * out_size);
    Aa = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_per_line * out_size * out_size);
    Bm = (int32_t *)bench_alloc(sizeof(int32_t) * out_size * out_size);
    Ba = (int32_t *)bench_alloc(sizeof(int32_t) * out_size * out_size);
    Cm = (int32_t *)bench_alloc(sizeof(int32_t) * out_size * out_size);
    Ca = (int32_t *)bench_alloc(sizeof(int32_t) * out_size * out_size);

    printf(" init A ...\n");
    for (int i=0; i < out_size * out_size; i++) {
      for (int j=0; j < reg_per_line * elem_per_reg; j++) {
        int16_t rand = bench_rand();
        int16_t temp = (bench_rand() % 2) ? -rand : rand;
        if (j < elem_per_line) {
          PutRawData(Am, i * reg_per_line * elem_per_reg + j, sew, temp);
          PutRawData(Aa, i * reg_per_line * elem_per_reg + j, sew, temp);
        }
        else {
          PutRawData(Am, i * reg_per_line * elem_per_reg + j, sew, elem_min[sew]);
          PutRawData(Aa, i * reg_per_line * elem_per_reg + j, sew, 0);
        }
      }
      if (i % 50 == 0)
        printf("  %d/%d \t", i, out_size * out_size);
    }

    uint64_t *img_addr;
    int temp, temp_std;

    // pool.max
    printf(" pool.max begin ...\n");
    pass1 = 1;
    for (int i=0; i < out_size * out_size; i++) {
      img_addr = (uint64_t *)&Am[reg_per_line * i];
      temp = elem_min[sew];
      for (int j=0; j < reg_per_line; j++) {
        temp = Pool_Max(img_addr[j], temp, sew);
        //printf("   i=%d:j=%d, tmp=%d ", i, j, temp);
      }
      Bm[i] = temp;

      temp_std = elem_min[sew];
      for (int j=0; j < elem_per_line; j++) {
        int data = GetRawData(Am, i * reg_per_line * elem_per_reg + j, sew);
        temp_std = (data > temp_std) ? data : temp_std;
        //printf("   i=%d:j=%d, std=%d ", i, j, temp_std);
      }
      Cm[i] = temp_std;

      if (Bm[i] != Cm[i]) {
        printf("  pool.max error: i=%d, poolmax_res=%d, std_res=%d\n", i, temp, temp_std);
        for (int si=0; si < reg_per_line * elem_per_reg; si++) {
          printf("  %d,", GetRawData(Am, i * reg_per_line * elem_per_reg + si, sew));
        }
        printf("\n");
        pass1 = 0;
      }
      else {
        ;//printf("  ok: i=%d, tmp_res=%d\n", i, temp);
      }
    }

    // pool.avg
    printf(" pool.avg begin ...\n");
    pass2 = 1;
    for (int i=0; i < out_size * out_size; i++) {
      img_addr = (uint64_t *)&Aa[reg_per_line * i];
      temp = 0;
      for (int j=0; j < reg_per_line; j++) {
        temp = Pool_Avg(img_addr[j], temp, sew);
        //printf("   i=%d:j=%d, tmp=%d ", i, j, temp);
      }
      Bm[i] = temp;

      temp_std = 0;
      for (int j=0; j < elem_per_line; j++) {
        temp_std += GetRawData(Am, i * reg_per_line * elem_per_reg + j, sew);
        //printf("   i=%d:j=%d, std=%d ", i, j, temp_std);
      }
      Cm[i] = temp_std;

      if (Bm[i] != Cm[i]) {
        printf("  pool.avg error: i=%d, poolavg_res=%d, std_res=%d\n", i, temp, temp_std);
        for (int si=0; si < reg_per_line * elem_per_reg; si++) {
          printf("  %d,", GetRawData(Am, i * reg_per_line * elem_per_reg + si, sew));
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

int bench_pool_validate() {
  return (setting->checksum == 0x00000011) && test_pass;
}
