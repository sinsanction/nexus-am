#include <benchmark.h>
#include "cvpinst.h"
#include "utils.h"

#define N_SIZE 28
#define CHANNEL 3
#define KERNEL_SIZE 3
#define STRIDE 1

static uint64_t *A, *kernel;
static int32_t *B, *C;
static int test_pass;
static int elem_bits[4] = {2, 4, 8, 16};

void bench_conv_prepare() {
  bench_srand(1);
  test_pass = 1;
}

void bench_conv_run() {
  int pass;
  int reg_per_line, elem_per_reg, elem_per_line;
  int out_size;
  uint8_t sew;

  for(sew=0; sew<=3; sew++) {
    out_size = (N_SIZE - KERNEL_SIZE) / STRIDE + 1;
    reg_per_line = RoundUpDiv(CHANNEL * KERNEL_SIZE * KERNEL_SIZE * elem_bits[sew], 64);
    elem_per_reg = 64 / elem_bits[sew];
    elem_per_line = CHANNEL * KERNEL_SIZE * KERNEL_SIZE;
    printf("\nsew=%d begin: reg_per_line=%d, elem_per_reg=%d, elem_per_line=%d, out_size=%d\n", sew, reg_per_line, elem_per_reg, elem_per_line, out_size);

    A = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_per_line * out_size * out_size);
    kernel = (uint64_t *)bench_alloc(sizeof(uint64_t) * reg_per_line);
    B = (int32_t *)bench_alloc(sizeof(int32_t) * out_size * out_size);
    C = (int32_t *)bench_alloc(sizeof(int32_t) * out_size * out_size);

    printf(" init A ...\n");
    int valid_elem_in_16b = RoundUpDiv(elem_per_line * elem_bits[sew], 16);
    int valid_bit_rem = elem_per_line * elem_bits[sew] % 16;
    int zero_bit = 16 - valid_bit_rem;
    int16_t *addr_a = (int16_t *)A;
    for (int i=0; i < out_size * out_size; i++) {
      for (int j=0; j < 4 * reg_per_line; j++) {
        if (j < valid_elem_in_16b) {
          int rand = bench_rand();
          *addr_a = (bench_rand() % 2) ? -rand : rand;
          if (j == (valid_elem_in_16b - 1) && valid_bit_rem != 0) {
            *addr_a = *addr_a & ((uint16_t)(~0) >> zero_bit);
          }
        }
        else {
          *addr_a = 0;
        }
        addr_a++;
      }
      if (i % 50 == 0)
        printf("  %d/%d \t", i, out_size * out_size);
    }

    printf(" init kernel ...\n");
    int16_t *addr_ker = (int16_t *)kernel;
    for (int i=0; i < 4 * reg_per_line; i++) {
      if (i < valid_elem_in_16b) {
        int rand = bench_rand();
        addr_ker[i] = (bench_rand() % 2) ? -rand : rand;
        if (i == (valid_elem_in_16b - 1) && valid_bit_rem != 0) {
          addr_ker[i] = addr_ker[i] & ((uint16_t)(~0) >> zero_bit);
        }
      }
      else
        addr_ker[i] = 0;
    }

    uint64_t *img_addr = (uint64_t *)A;
    uint64_t *ker_addr = (uint64_t *)kernel;
    pass = 1;

    for (int i=0; i < reg_per_line * elem_per_reg; i++) {
      printf("  %d,", GetRawData(kernel, i, sew));
    }
    printf("\n kernel end\n");

    int temp, temp_std;
    for (int i=0; i < out_size * out_size; i++) {
      ker_addr = (uint64_t *)kernel;
      temp = 0;
      for (int j=0; j < reg_per_line; j++) {
        temp = Conv(*img_addr, *ker_addr, temp, sew);
        img_addr++;
        ker_addr++;
        //printf("   i=%d:j=%d, tmp=%d ", i, j, temp);
      }
      B[i] = temp;

      temp_std = 0;
      for (int j=0; j < elem_per_line; j++) {
        temp_std += GetRawData(A, i * reg_per_line * elem_per_reg + j, sew) * GetRawData(kernel, j, sew);
        //printf("   i=%d:j=%d, std=%d ", i, j, temp_std);
      }
      C[i] = temp_std;

      if (B[i] != C[i]) {
        printf("  conv error: i=%d, conv_res=%d, std_res=%d\n", i, temp, temp_std);
        for (int si=0; si < reg_per_line * elem_per_reg; si++) {
          printf("  %d,", GetRawData(A, i * reg_per_line * elem_per_reg + si, sew));
        }
        printf("\n");
        pass = 0;
      }
      else {
        ;//printf("  ok: i=%d, tmp_res=%d\n", i, temp);
      }
    }

    if (pass == 1) {
      printf("sew=%d end: pass!!!\n", sew);
    }
    else {
      printf("sew=%d end: fail\n", sew);
      test_pass = 0;
    }
    bench_free(A);
    bench_free(B);
    bench_free(C);
    bench_free(kernel);
  }
}

int bench_conv_validate() {
  return (setting->checksum == 0x00000001) && test_pass;
}
