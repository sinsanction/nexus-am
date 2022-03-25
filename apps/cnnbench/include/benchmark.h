#ifndef __BENCHMARK_H__
#define __BENCHMARK_H__

#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MB * 1024 * 1024
#define KB * 1024

#define REPEAT  1

//                  size |  heap  |  checksum
#define HELLO   {     100,   1 KB,  0x00000000}
#define CONV_16 {      20, 128 KB,  0x00000001}
#define CONV_8  {      20,   1 KB,  0x00000002}
#define CONV_4  {      20,   1 KB,  0x00000003}
#define CONV_2  {      20,   1 KB,  0x00000004}
#define POOL_16 {      20, 128 KB,  0x00000011}
#define ACT_16  {      20, 128 KB,  0x00000021}

#define BENCHMARK_LIST(def) \
  def(hello,  "hello",  HELLO,   "hello world") \
  def(conv16, "conv16", CONV_16, "CNN Conv 16b") \
  def(pool16, "pool16", POOL_16, "CNN Pool 16b") \
  def(act16,  "act16",  ACT_16,  "CNN Act 16b")

// Each benchmark will run REPEAT times

#define DECL(_name, _sname, _setting, _desc) \
  void bench_##_name##_prepare(); \
  void bench_##_name##_run(); \
  int bench_##_name##_validate();

BENCHMARK_LIST(DECL)

typedef struct Setting {
  int size;
  unsigned long mlim;
  uint32_t checksum;
} Setting;

typedef struct Benchmark {
  void (*prepare)();
  void (*run)();
  int (*validate)();
  const char *name, *desc;
  Setting settings;
} Benchmark;

extern Benchmark *current;
extern Setting *setting;

typedef struct Result {
  int pass;
  unsigned long tsc, msec;
} Result;

void prepare(Result *res);
void done(Result *res);

// memory allocation
void* bench_alloc(size_t size);
void bench_free(void *ptr);

// random number generator
void bench_srand(uint32_t seed);
uint32_t bench_rand(); // return a random number between 0..32767

// checksum
uint32_t checksum(void *start, void *end);

#ifdef __cplusplus
}
#endif

#endif
