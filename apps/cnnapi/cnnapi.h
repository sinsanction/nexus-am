#ifndef __CNNAPI_H__
#define __CNNAPI_H__

#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CHANNEL 50

typedef struct image
{
  uint32_t width;
  uint32_t height;

  uint8_t vwidth;
  uint8_t order;         // 0: row  1: column

  void *addr;
} image_t;

typedef struct kernel
{
  uint32_t size;
  uint8_t vwidth;

  uint16_t den;

  void *addr;
} kernel_t;

typedef struct fc_filter
{
  uint32_t width;
  uint32_t height;

  uint8_t vwidth;
  uint8_t order;
  uint16_t den;

  void *addr;
} fc_filter_t;

typedef struct image_mc
{
  uint32_t width;
  uint32_t height;
  uint16_t channel;

  uint8_t order;

  image_t *img[MAX_CHANNEL];
} image_mc_t;

typedef struct kernel_mc
{
  uint32_t size;
  uint16_t channel;

  kernel_t *ker[MAX_CHANNEL];
} kernel_mc_t;

inline int round_up_div(int a, int b);
inline uint16_t get_main_value(uint64_t *img_addr, int i, uint8_t vwidth);
inline int8_t get_kernel_value(uint64_t *kernel_addr, int i, uint8_t vwidth);
inline void put_main_value(uint64_t *img_addr, int i, uint8_t vwidth, uint16_t value);

//IO
image_t *RandomInitImage_SC(uint32_t width, uint32_t height, uint32_t bits, uint8_t order);

kernel_t *RandomInitKernel_SC(uint32_t k, uint32_t bits);

image_mc_t *RandomInitImage(uint32_t width, uint32_t height, uint32_t bits, uint16_t channel);

kernel_mc_t *RandomInitKernel(uint32_t k, uint32_t bits, uint16_t channel);

fc_filter_t *RandomInitFcFilter(uint32_t width, uint32_t height, uint32_t bits);

fc_filter_t *RandomInitFcFilterArray(uint32_t width, uint32_t height, uint32_t bits, int units);

void SetOutput_SC(image_t *output_image);

void SetOutputKernel_SC(kernel_t *output_kernel);

void SetOutput(image_mc_t *output_image);

void SetOutputKernel(kernel_mc_t *output_kernel);

void SetOutputFcFilter(fc_filter_t *output_fc_filter);

//utils
image_t *Transpose(image_t *input_image);

image_t *MergeImage(image_t *input_image_a, image_t *input_image_b);

//arithmetic
image_t *Convolution_SC(image_t *input_image, kernel_t *input_kernel, int strides);

image_t *MaxPooling_SC(image_t *input_image, int pool_size, int strides);

image_t *AvgPooling_SC(image_t *input_image, int pool_size, int strides);

image_t *Activation_SC(image_t *input_image, char *algorithm, uint16_t zero_point);


image_mc_t *Convolution(image_mc_t *input_image, kernel_mc_t *input_kernel, int strides);

image_mc_t *MaxPooling(image_mc_t *input_image, int pool_size, int strides);

image_mc_t *AvgPooling(image_mc_t *input_image, int pool_size, int strides);

image_mc_t *Activation(image_mc_t *input_image, char *algorithm, uint16_t zero_point);

image_t *Flatten(image_mc_t *input_image);

image_t *Dense(image_t *input_image, fc_filter_t *fc_filter_array, int units);

#ifdef __cplusplus
}
#endif

#endif
