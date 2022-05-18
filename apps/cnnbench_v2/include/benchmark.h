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
#define CONV_16 {     100, 512 KB,  0x00000001}
#define CONV_8  {     100, 256 KB,  0x00000002}
#define CONV_4  {     100, 256 KB,  0x00000003}
#define CONV_2  {     100, 256 KB,  0x00000004}

#define POOL_16 {     100, 512 KB,  0x00000011}
#define POOL_8  {     100, 256 KB,  0x00000012}
#define POOL_4  {     100, 256 KB,  0x00000013}
#define POOL_2  {     100, 256 KB,  0x00000014}

#define ACT_16  {     100, 256 KB,  0x00000021}
#define ACT_8   {     100, 256 KB,  0x00000022}
#define ACT_4   {     100, 256 KB,  0x00000023}
#define ACT_2   {     100, 256 KB,  0x00000024}

#define BENCHMARK_LIST(def) \
  def(conv16, "conv16", CONV_16, "CNN Conv 16b") \
  def(conv8,  "conv8",  CONV_8,  "CNN Conv 8b") \
  def(conv4,  "conv4",  CONV_4,  "CNN Conv 4b") \
  def(conv2,  "conv2",  CONV_2,  "CNN Conv 2b") 

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
