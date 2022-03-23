#include <benchmark.h>

static int n;

void bench_hello_prepare() {
  bench_srand(1);

  n = setting->size;
}

static void myhello(int n) {
  if (n == 100){
    printf("\nhello world!\n");
  }
  else {
    printf("\nhello error\n");
  }
}

void bench_hello_run() {
  myhello(n);
}

int bench_hello_validate() {
  return setting->checksum == 0;
}
