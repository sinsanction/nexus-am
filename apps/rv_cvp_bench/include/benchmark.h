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
#define CONV    {     100,   2 MB,  0x00000001}
#define POOL    {     100,   2 MB,  0x00000011}
#define ACT     {     100,   1 MB,  0x00000021}
#define ADD     {     100,   1 MB,  0x00000031}
#define POOLM   {     100,   2 MB,  0x00000041}

#define BENCHMARK_LIST(def) \
  def(conv,  "conv",  CONV,  "CNN Conv") \
  def(pool,  "pool",  POOL,  "CNN Pool") \
  def(act,   "act",   ACT,   "CNN Act") \
  def(add,   "add",   ADD,   "CNN Add") \
  def(poolm, "poolm", POOLM, "CNN PoolM")

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
