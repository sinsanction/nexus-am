#include <benchmark.h>
#include "cnn.h"

#define N N_SIZE
#define S 1

static uint16_t A[N][N];
static uint64_t vwidth;
static int test_pass;

void bench_conv16_perf_prepare() {
  bench_srand(1);
  vwidth = 0x8888888888888888;
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      A[i][j] = bench_rand() & 0xff;
    }
  }
  test_pass = 1;
}

void bench_conv16_perf_run() {
  int k;              //kernel size
  int m;              //output size
  uint16_t *B;        //cnn output
  int8_t *kernel;     //kernel

  for (k=1; k<=5; k++) {
    m = (N - k) / S + 1;
    B = (uint16_t *)bench_alloc(sizeof(uint16_t) * m * m);
    kernel = (int8_t *)bench_alloc(sizeof(int8_t) * k * k);

    for (int i=0; i<k*k; i++) {
      kernel[i] = bench_rand() & 0xff;
    }

    LoadV_Width((uint64_t)&vwidth);

    int8_t *kernel_ptr = kernel;
    for (int i=0; i<k; i++) {
      LoadV_D_Kernel((uint64_t)(kernel_ptr), k, i, 0);
      kernel_ptr += k;
    }

    uint16_t *col_ptr;
    for (int i=0; i<m; i++) {
      col_ptr = &A[0][i];
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

int bench_conv16_perf_validate() {
  return (setting->checksum == 0x00020001) && test_pass;
}
