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
#define CONV_8  {      20,  64 KB,  0x00000002}
#define CONV_4  {      20,  64 KB,  0x00000003}
#define CONV_2  {      20,  32 KB,  0x00000004}

#define POOL_16 {      20, 128 KB,  0x00000011}
#define POOL_8  {      20,  64 KB,  0x00000012}
#define POOL_4  {      20,  64 KB,  0x00000013}
#define POOL_2  {      20,  32 KB,  0x00000014}

#define ACT_16  {      20, 128 KB,  0x00000021}
#define ACT_8   {      20,  64 KB,  0x00000022}
#define ACT_4   {      20,  64 KB,  0x00000023}
#define ACT_2   {      20,  32 KB,  0x00000024}

#define BENCHMARK_LIST(def) \
  def(hello,  "hello",  HELLO,   "hello world") \
  def(conv16, "conv16", CONV_16, "CNN Conv 16b") \
  def(pool16, "pool16", POOL_16, "CNN Pool 16b") \
  def(act16,  "act16",  ACT_16,  "CNN Act 16b") \
  def(conv8, "conv8", CONV_8, "CNN Conv 8b") \
  def(pool8, "pool8", POOL_8, "CNN Pool 8b") \
  def(act8,  "act8",  ACT_8,  "CNN Act 8b") \
  def(conv4, "conv4", CONV_4, "CNN Conv 4b") \
  def(pool4, "pool4", POOL_4, "CNN Pool 4b") \
  def(act4,  "act4",  ACT_4,  "CNN Act 4b") \
  def(conv2, "conv2", CONV_2, "CNN Conv 2b") \
  def(pool2, "pool2", POOL_2, "CNN Pool 2b") \
  def(act2,  "act2",  ACT_2,  "CNN Act 2b")

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
