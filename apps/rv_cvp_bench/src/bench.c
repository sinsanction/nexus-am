#include <am.h>
#include <benchmark.h>
#include <limits.h>
#include <klib-macros.h>

Benchmark *current;
Setting *setting;

static char *hbrk;

// The benchmark list

#define ENTRY(_name, _sname, _setting, _desc) \
  { .prepare = bench_##_name##_prepare, \
    .run = bench_##_name##_run, \
    .validate = bench_##_name##_validate, \
    .name = _sname, \
    .desc = _desc, \
    .settings = _setting, },

Benchmark benchmarks_func[] = {
  BENCHMARK_LIST(ENTRY)
};

// Running a benchmark
static void bench_prepare(Result *res) {
  res->msec = uptime();
}

static void bench_reset() {
  hbrk = (void *)ROUNDUP(_heap.start, 8);
}

static void bench_done(Result *res) {
  res->msec = uptime() - res->msec;
}

static const char *bench_check(Benchmark *bench) {
  uintptr_t freesp = (uintptr_t)_heap.end - (uintptr_t)_heap.start;
  if (freesp < setting->mlim) {
    return "(insufficient memory)";
  }
  return NULL;
}

static void run_once(Benchmark *b, Result *res) {
  bench_reset();       // reset malloc state
  current->prepare();  // call bechmark's prepare function
  bench_prepare(res);  // clean everything, start timer
  current->run();      // run it
  bench_done(res);     // collect results
  res->pass = current->validate();
}

int main(const char *args) {
  const char *setting_name = args;
  if (args == NULL || strcmp(args, "") == 0) {
    printf("Empty mainargs. Use \"testfunc\" by default\n");
    setting_name = "testfunc";
  }
  int setting_id = -1;

  if      (strcmp(setting_name, "testfunc" ) == 0) setting_id = 0;
  else {
    printf("Invalid mainargs: \"%s\"; "
           "must be in {testfunc}\n", setting_name);
    _halt(1);
  }

  _ioe_init();

  printf("======= Running RVCVPMicroBench [input *%s* id:%d] =======\n", setting_name, setting_id);

  int pass = 1;
  uint32_t t0 = uptime();
  int bench_length;
  Benchmark *test_bench;
  bench_length = LENGTH(benchmarks_func);
  test_bench = benchmarks_func;

  for (int i = 0; i < bench_length; i ++) {
    Benchmark *bench = &test_bench[i];
    current = bench;
    setting = &bench->settings;
    const char *msg = bench_check(bench);
    printf("\n[%s] %s: ", bench->name, bench->desc);
    if (msg != NULL) {
      printf("Ignored %s\n", msg);
    } else {
      unsigned long msec = ULONG_MAX;
      int succ = 1;
      for (int i = 0; i < REPEAT; i ++) {
        Result res;
        run_once(bench, &res);
        printf(res.pass ? "*" : "X");
        succ &= res.pass;
        if (res.msec < msec) msec = res.msec;
      }

      if (succ) printf(" Passed.");
      else printf(" Failed.");

      pass &= succ;

      printf("\n");
      printf("  min time: %d ms\n", (unsigned int)msec);
    }
  }
  uint32_t t1 = uptime();

  printf("==================================================\n");
  printf("RVCVPBench %s", pass ? "PASS" : "FAIL");
  printf("\n");
  printf("Total time: %d ms\n", t1 - t0);
  return !pass;
}

// Libraries

void* bench_alloc(size_t size) {
  size  = (size_t)ROUNDUP(size, 8);
  char *old = hbrk;
  hbrk += size;
  assert((uintptr_t)_heap.start <= (uintptr_t)hbrk && (uintptr_t)hbrk < (uintptr_t)_heap.end);
  for (uint64_t *p = (uint64_t *)old; p != (uint64_t *)hbrk; p ++) {
    *p = 0;
  }
  assert((uintptr_t)hbrk - (uintptr_t)_heap.start <= setting->mlim);
  return old;
}

void bench_free(void *ptr) {
}

static uint32_t seed = 1;

void bench_srand(uint32_t _seed) {
  seed = _seed & 0x7fff;
}

uint32_t bench_rand() {
  seed = (seed * (uint32_t)214013L + (uint32_t)2531011L);
  return (seed >> 16) & 0x7fff;
}

// FNV hash
uint32_t checksum(void *start, void *end) {
  const uint32_t x = 16777619;
  uint32_t h1 = 2166136261u;
  for (uint8_t *p = (uint8_t*)start; p + 4 < (uint8_t*)end; p += 4) {
    for (int i = 0; i < 4; i ++) {
      h1 = (h1 ^ p[i]) * x;
    }
  }
  int32_t hash = (uint32_t)h1;
  hash += hash << 13;
  hash ^= hash >> 7;
  hash += hash << 3;
  hash ^= hash >> 17;
  hash += hash << 5;
  return hash;
}
