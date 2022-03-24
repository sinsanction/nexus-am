#include <benchmark.h>
#include "cnn.h"

#define N 10
#define S 1

static uint16_t A[N][N];
static uint64_t vwidth;

void bench_conv16_prepare() {
  bench_srand(1);
  vwidth = 0x8888888888888888;
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      A[i][j] = bench_rand() & 0xff;
    }
  }
}

void bench_conv16_run() {
  int k;              //kernel size
  int m;              //output size
  uint16_t *B;        //cnn output
  uint16_t *C;        //std output
  int8_t *kernel;     //kernel
  
  k = 3;
  m = (N - k) / S + 1;
  B = (uint16_t *)bench_alloc(sizeof(uint16_t) * m * m);
  C = (uint16_t *)bench_alloc(sizeof(uint16_t) * m * m);
  kernel = (int8_t *)bench_alloc(sizeof(int8_t) * k * k);

  kernel[0] = 10;
  kernel[1] = -10;
  kernel[2] = 10;
  kernel[3] = -10;
  kernel[4] = 10;
  kernel[5] = -10;
  kernel[6] = 10;
  kernel[7] = -10;
  kernel[8] = 10;
  printf("\n");
  for (int i=0; i<k*k; i++){
    //kernel[i] = bench_rand() & 0xff;
    printf("%d ", kernel[i]);
  }
  printf("\n");

  LoadV_Width((uint64_t)&vwidth);

  LoadV_D_Kernel((uint64_t)(kernel), k, 0, 0);
  LoadV_D_Kernel((uint64_t)(kernel+3), k, 1, 0);
  LoadV_D_Kernel((uint64_t)(kernel+6), k, 2, 0);

  uint16_t *col_ptr;
  for (int i=0; i<m; i++) {
    col_ptr = &A[0][i];
    LoadV_D_Main((uint64_t)(col_ptr), k, 0, 0);
    LoadV_D_Main((uint64_t)(col_ptr+N), k, 1, 0);
    LoadV_D_Main((uint64_t)(col_ptr+2*N), k, 2, 0);
    B[0 * m + i] = Conv(k);

    // std res
    int32_t tmp_res = 0;
    for (int si=0; si<k; si++) {
      for (int sj=0; sj<k; sj++) {
        tmp_res += A[0 + sj][i + si] * kernel[si * k + sj];
      }
    }
    if (tmp_res < 0) {
      C[0 * m + i] = 0;
    }
    else if (tmp_res > 0xffff) {
      C[0 * m + i] = 0xffff;
    }
    else {
      C[0 * m + i] = tmp_res;
    }

    if (B[0 * m + i] != C[0 * m + i]) {
      printf("conv error: i=%d, j=0, conv_res=%d, std_res=%d, tmp_res=%d\n", i, B[0 * m + i], C[0 * m + i], tmp_res);
      for (int si=0; si<k; si++) {
        for (int sj=0; sj<k; sj++) {
          printf("%d ", A[0 + sj][i + si]);
        }
      }
      printf("\n");
    }
    else {
      printf("ok: i=%d, j=0, std_res=%d, tmp_res=%d\n", i, C[0 * m + i], tmp_res);
    }

    col_ptr += 3*N;
    for (int j=1; j<m; j++) {
      LoadV_P((uint64_t)(col_ptr), k, 0);
      B[j * m + i] = Conv(k);

      // std res
      int32_t tmp_res = 0;
      for (int si=0; si<k; si++) {
        for (int sj=0; sj<k; sj++) {
          tmp_res += A[j + sj][i + si] * kernel[si * k + sj];
        }
      }
      if (tmp_res < 0) {
        C[j * m + i] = 0;
      }
      else if (tmp_res > 0xffff) {
        C[j * m + i] = 0xffff;
      }
      else {
        C[j * m + i] = tmp_res;
      }

      if (B[j * m + i] != C[j * m + i]) {
        printf("conv error: i=%d, j=%d, conv_res=%d, std_res=%d, tmp_res=%d\n", i, j, B[j * m + i], C[j * m + i], tmp_res);
        for (int si=0; si<k; si++) {
          for (int sj=0; sj<k; sj++) {
            printf("%d ", A[j + sj][i + si]);
          }
        }
        printf("\n");
      }
      else {
        printf("ok: i=%d, j=%d, std_res=%d, tmp_res=%d\n", i, j, C[j * m + i], tmp_res);
      }

      col_ptr += N;
    }
  }
}

int bench_conv16_validate() {
  return setting->checksum == 1;
}
