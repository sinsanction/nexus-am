#include <benchmark.h>
#include "cvpinst.h"

#define WIDTH 28
#define HEIGHT 28
#define CHANNEL 5
#define KERNEL_SIZE 3
#define STRIDE 1
#define OUT_WIDTH ((WIDTH - KERNEL_SIZE) / STRIDE + 1)
#define OUT_HEIGHT ((HEIGHT - KERNEL_SIZE) / STRIDE + 1)
#define SEW 0x3

static int16_t *A, *kernel;
static int32_t *B, *C;
static int test_pass;
static int single_num;

inline int RoundUpDiv(int a, int b) {
  int div = a / b;
  int rem = a % b;
  return (rem == 0) ? div : div + 1;
}

void bench_conv16_prepare() {
  single_num = RoundUpDiv(CHANNEL * KERNEL_SIZE * KERNEL_SIZE * (1 << (SEW + 1)), 64);
  printf("\nsingle_num=%d: \n", single_num);
  bench_srand(1);

  A = (int16_t *)bench_alloc(sizeof(int64_t) * single_num * OUT_WIDTH * OUT_HEIGHT);
  B = (int32_t *)bench_alloc(sizeof(int32_t) * OUT_WIDTH * OUT_HEIGHT);
  C = (int32_t *)bench_alloc(sizeof(int32_t) * OUT_WIDTH * OUT_HEIGHT);
  for (int i=0; i < OUT_WIDTH * OUT_HEIGHT; i++) {
    for (int j=0; j < 4 * single_num; j++) {
      if (j < CHANNEL * KERNEL_SIZE * KERNEL_SIZE) {
        int rand = bench_rand();
        int sign = bench_rand() % 2;
        A[i*single_num*4 + j] = sign ? rand | 0x8000 : rand;
      }
      else
        A[i*single_num*4 + j] = 0;
    }
  }

  kernel = (int16_t *)bench_alloc(sizeof(int16_t) * CHANNEL * KERNEL_SIZE * KERNEL_SIZE);
  for (int j=0; j < 4 * single_num; j++) {
    if (j < CHANNEL * KERNEL_SIZE * KERNEL_SIZE) {
      int rand = bench_rand();
      int sign = bench_rand() % 2;
      kernel[j] = sign ? rand | 0x8000 : rand;
    }
    else
      kernel[j] = 0;
  }

  test_pass = 1;
}

void bench_conv16_run() {
  int pass = 1;
  uint64_t *img_addr = (uint64_t *)A;
  uint64_t *ker_addr = (uint64_t *)kernel;

  printf("\nk=%d begin: \n", KERNEL_SIZE);
  for (int i=0; i < CHANNEL * KERNEL_SIZE * KERNEL_SIZE; i++) {
    printf("  %d", kernel[i]);
  }
  printf("\n");

  for (int i=0; i < OUT_WIDTH * OUT_HEIGHT; i++) {
    int temp = 0;
    ker_addr = (uint64_t *)kernel;
    for (int j=0; j < single_num; j++) {
      temp = Conv(*(img_addr++), *(ker_addr++), temp, SEW);
    }
    B[i] = temp;

    int temp_std = 0;
    for (int j=0; j < CHANNEL * KERNEL_SIZE * KERNEL_SIZE; j++) {
      temp_std += A[i*single_num*4 + j] * kernel[j];
    }
    C[i] = temp_std;

    if (B[i] != C[i]) {
      printf("  conv error: i=%d, conv_res=%d, std_res=%d, std_res=%d, tmp_res=%d\n", i, B[i], C[ i], temp, temp_std);
      for (int si=0; si < CHANNEL * KERNEL_SIZE * KERNEL_SIZE; si++) {
        printf("  %d", A[i*single_num*4 + si]);
      }
      printf("\n");
      pass = 0;
    }
    else {
      ;//printf("  ok: i=%d, tmp_res=%d\n", i, B[i]);
    }
  }

  if (pass == 1) {
    printf("k=%d end: pass!!!\n", KERNEL_SIZE);
  }
  else {
    printf("k=%d end: fail\n", KERNEL_SIZE);
    test_pass = 0;
  }
  bench_free(A);
  bench_free(B);
  bench_free(C);
  bench_free(kernel);
}

int bench_conv16_validate() {
  return (setting->checksum == 0x00000001) && test_pass;
}
