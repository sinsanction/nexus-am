#include <benchmark.h>
#include "cnn.h"

#define N N_SIZE
#define S 1

static uint8_t A[N][N];
static uint64_t vwidth;
static int test_pass;

void bench_pool8_perf_prepare() {
  bench_srand(1);
  vwidth = 0x4444444444444444;
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      A[i][j] = bench_rand() & 0xff;
    }
  }
  test_pass = 1;
}

void bench_pool8_perf_run() {
  int k;              //kernel size
  int m;              //output size
  uint8_t *Bm;        //pool max output
  uint8_t *Ba;        //pool avg output

  for (k=1; k<=5; k++) {
    m = (N - k) / S + 1;
    Bm = (uint8_t *)bench_alloc(sizeof(uint8_t) * m * m);
    Ba = (uint8_t *)bench_alloc(sizeof(uint8_t) * m * m);

    LoadV_Width((uint64_t)&vwidth);

    uint8_t *col_ptr;
    for (int i=0; i<m; i++) {
      col_ptr = &A[0][i];
      for (int l=0; l<k; l++) {
        LoadV_D_Main((uint64_t)(col_ptr), k, l, 0);
        col_ptr += N;
      }
      Bm[0 * m + i] = Pool_Max(k);
      Ba[0 * m + i] = Pool_Avg(k);

      for (int j=1; j<m; j++) {
        LoadV_P((uint64_t)(col_ptr), k, 0);
        Bm[j * m + i] = Pool_Max(k);
        Ba[j * m + i] = Pool_Avg(k);
        col_ptr += N;
      }
    }

    bench_free(Bm);
    bench_free(Ba);
  }
}

int bench_pool8_perf_validate() {
  return (setting->checksum == 0x00020012) && test_pass;
}
