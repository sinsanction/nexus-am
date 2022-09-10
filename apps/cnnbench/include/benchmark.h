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
#define CONV_16 {     100,   2 MB,  0x00000001}
#define CONV_8  {     100,   1 MB,  0x00000002}
#define CONV_4  {     100,   1 MB,  0x00000003}
#define CONV_2  {     100,   1 MB,  0x00000004}

#define POOL_16 {     100, 512 KB,  0x00000011}
#define POOL_8  {     100, 256 KB,  0x00000012}
#define POOL_4  {     100, 256 KB,  0x00000013}
#define POOL_2  {     100, 256 KB,  0x00000014}

#define ACT_16  {     100, 256 KB,  0x00000021}
#define ACT_8   {     100, 256 KB,  0x00000022}
#define ACT_4   {     100, 256 KB,  0x00000023}
#define ACT_2   {     100, 256 KB,  0x00000024}

//base
#define CONV_16_BASE {     100,   1 MB,  0x00010001}
#define CONV_8_BASE  {     100,   1 MB,  0x00010002}
#define CONV_4_BASE  {     100, 512 KB,  0x00010003}
#define CONV_2_BASE  {     100, 512 KB,  0x00010004}

#define POOL_16_BASE {     100, 256 KB,  0x00010011}
#define POOL_8_BASE  {     100, 256 KB,  0x00010012}
#define POOL_4_BASE  {     100, 128 KB,  0x00010013}
#define POOL_2_BASE  {     100, 128 KB,  0x00010014}

#define ACT_16_BASE  {     100, 256 KB,  0x00010021}
#define ACT_8_BASE   {     100, 256 KB,  0x00010022}
#define ACT_4_BASE   {     100, 128 KB,  0x00010023}
#define ACT_2_BASE   {     100, 128 KB,  0x00010024}

//perf
#define CONV_16_PERF {     100,   1 MB,  0x00020001}
#define CONV_8_PERF  {     100,   1 MB,  0x00020002}
#define CONV_4_PERF  {     100, 512 KB,  0x00020003}
#define CONV_2_PERF  {     100, 512 KB,  0x00020004}

#define POOL_16_PERF {     100, 256 KB,  0x00020011}
#define POOL_8_PERF  {     100, 256 KB,  0x00020012}
#define POOL_4_PERF  {     100, 128 KB,  0x00020013}
#define POOL_2_PERF  {     100, 128 KB,  0x00020014}

#define ACT_16_PERF  {     100, 256 KB,  0x00020021}
#define ACT_8_PERF   {     100, 256 KB,  0x00020022}
#define ACT_4_PERF   {     100, 128 KB,  0x00020023}
#define ACT_2_PERF   {     100, 128 KB,  0x00020024}

#define BENCHMARK_LIST(def) \
  def(conv16, "conv16", CONV_16, "CNN Conv 16b") \
  def(pool16, "pool16", POOL_16, "CNN Pool 16b") \
  def(act16,  "act16",  ACT_16,  "CNN Act 16b") \
  def(conv8,  "conv8",  CONV_8,  "CNN Conv 8b") \
  def(pool8,  "pool8",  POOL_8,  "CNN Pool 8b") \
  def(act8,   "act8",   ACT_8,   "CNN Act 8b") \
  def(conv4,  "conv4",  CONV_4,  "CNN Conv 4b") \
  def(pool4,  "pool4",  POOL_4,  "CNN Pool 4b") \
  def(act4,   "act4",   ACT_4,   "CNN Act 4b") \
  def(conv2,  "conv2",  CONV_2,  "CNN Conv 2b") \
  def(pool2,  "pool2",  POOL_2,  "CNN Pool 2b") \
  def(act2,   "act2",   ACT_2,   "CNN Act 2b")

#define BENCHMARK_BASE_LIST(def) \
  def(conv16_base, "conv16_base", CONV_16_BASE, "CNN Conv 16b base") \
  def(pool16_base, "pool16_base", POOL_16_BASE, "CNN Pool 16b base") \
  def(act16_base,  "act16_base",  ACT_16_BASE,  "CNN Act 16b base") \
  def(conv8_base,  "conv8_base",  CONV_8_BASE,  "CNN Conv 8b base") \
  def(pool8_base,  "pool8_base",  POOL_8_BASE,  "CNN Pool 8b base") \
  def(act8_base,   "act8_base",   ACT_8_BASE,   "CNN Act 8b base") \
  def(conv4_base,  "conv4_base",  CONV_4_BASE,  "CNN Conv 4b base") \
  def(pool4_base,  "pool4_base",  POOL_4_BASE,  "CNN Pool 4b base") \
  def(act4_base,   "act4_base",   ACT_4_BASE,   "CNN Act 4b base") \
  def(conv2_base,  "conv2_base",  CONV_2_BASE,  "CNN Conv 2b base") \
  def(pool2_base,  "pool2_base",  POOL_2_BASE,  "CNN Pool 2b base") \
  def(act2_base,   "act2_base",   ACT_2_BASE,   "CNN Act 2b base")

#define BENCHMARK_PERF_LIST(def) \
  def(conv16_perf, "conv16_perf", CONV_16_PERF, "CNN Conv 16b perf") \
  def(pool16_perf, "pool16_perf", POOL_16_PERF, "CNN Pool 16b perf") \
  def(act16_perf,  "act16_perf",  ACT_16_PERF,  "CNN Act 16b perf") \
  def(conv8_perf,  "conv8_perf",  CONV_8_PERF,  "CNN Conv 8b perf") \
  def(pool8_perf,  "pool8_perf",  POOL_8_PERF,  "CNN Pool 8b perf") \
  def(act8_perf,   "act8_perf",   ACT_8_PERF,   "CNN Act 8b perf") \
  def(conv4_perf,  "conv4_perf",  CONV_4_PERF,  "CNN Conv 4b perf") \
  def(pool4_perf,  "pool4_perf",  POOL_4_PERF,  "CNN Pool 4b perf") \
  def(act4_perf,   "act4_perf",   ACT_4_PERF,   "CNN Act 4b perf") \
  def(conv2_perf,  "conv2_perf",  CONV_2_PERF,  "CNN Conv 2b perf") \
  def(pool2_perf,  "pool2_perf",  POOL_2_PERF,  "CNN Pool 2b perf") \
  def(act2_perf,   "act2_perf",   ACT_2_PERF,   "CNN Act 2b perf")

// Each benchmark will run REPEAT times

#define DECL(_name, _sname, _setting, _desc) \
  void bench_##_name##_prepare(); \
  void bench_##_name##_run(); \
  int bench_##_name##_validate();

BENCHMARK_LIST(DECL)
BENCHMARK_BASE_LIST(DECL)
BENCHMARK_PERF_LIST(DECL)

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
