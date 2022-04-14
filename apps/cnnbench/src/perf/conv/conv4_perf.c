#include <benchmark.h>
#include "cnn.h"

#define N N_SIZE
#define S 1

static uint8_t *A;
static uint64_t vwidth;
static int test_pass;
static int a_size;

inline int round_up_div(int a, int b) {
  int div = a / b;
  int rem = a % b;
  if (rem == 0) {
    return div;
  }
  else {
    return div + 1;
  }
}

void bench_conv4_perf_prepare() {
  bench_srand(1);
  vwidth = 0x2222222222222222;
  a_size = round_up_div(N * N, 2);
  A = (uint8_t *)bench_alloc(sizeof(uint8_t) * a_size);
  for (int i=0; i<a_size; i++) {
    A[i] = bench_rand() & 0xff;
  }
  test_pass = 1;
}

void bench_conv4_perf_run() {
  int k;              //kernel size
  int m;              //output size
  uint8_t *B;         //cnn output
  int8_t *kernel;     //kernel
  int k_size;

  for (k=1; k<=5; k++) {
    m = (N - k) / S + 1;
    B = (uint8_t *)bench_alloc(sizeof(uint8_t) * m * m);
    k_size = round_up_div(k * k, 4);
    kernel = (int8_t *)bench_alloc(sizeof(int8_t) * k_size);

    for (int i=0; i<k_size; i++) {
      kernel[i] = bench_rand() & 0xff;
    }

    LoadV_Width((uint64_t)&vwidth);

    uint64_t kernel_ptr = ((uint64_t)kernel) << 2;
    for (int i=0; i<k; i++) {
      LoadV_D_Kernel(kernel_ptr, k, i, 0);
      kernel_ptr += k;
    }

    uint64_t col_ptr;
    for (int i=0; i<m; i++) {
      col_ptr = (((uint64_t)A) << 1) + i;
      for (int l=0; l<k; l++) {
        LoadV_D_Main((uint64_t)(col_ptr), k, l, 0);
        col_ptr += N;
      }
      B[0 * m + i] = Conv(k);

      for (int j=1; j<m; j++) {
        LoadV_P((uint64_t)(col_ptr), k, 0);
        B[j * m + i] = Conv(k);
        col_ptr += N;
      }
    }

    bench_free(B);
    bench_free(kernel);
  }
}

int bench_conv4_perf_validate() {
  bench_free(A);
  return (setting->checksum == 0x00020003) && test_pass;
}
