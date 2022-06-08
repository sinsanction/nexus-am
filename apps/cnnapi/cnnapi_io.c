#include "cnnapi.h"


// IO
image_t *RandomInit(uint32_t width, uint32_t height, uint32_t bits, uint8_t order) {

  image_t *img = (image_t *)malloc(sizeof(image_t));
  img->width = width;
  img->height = height;
  img->order = order;

  if (bits == 16) {
    img->vwidth = 0x80;
  }
  else if (bits == 8) {
    img->vwidth = 0x40;
  }
  else if (bits == 4) {
    img->vwidth = 0x20;
  }
  else if (bits == 2) {
    img->vwidth = 0x10;
  }
  else {
    free(img);
    return NULL;
  }

  int size = round_up_div(width * height * bits, 64);
  uint64_t *img_data = (uint64_t *)malloc(sizeof(uint64_t) * size);
  uint16_t *data = (uint16_t *)img_data;
  for (int i=0; i<size*4; i++) {
    // RAND_MAX assumed to be 32767
    data[i] = rand();
  }
  img->addr = (void *)img_data;

  return img;
}

kernel_t *RandomInitKernel(uint32_t k, uint32_t bits) {

  kernel_t *kernel = (kernel_t *)malloc(sizeof(kernel_t));
  kernel->size = k;

  if (bits == 8) {
    kernel->vwidth = 0x8;
    kernel->den = 128;
  }
  else if (bits == 4) {
    kernel->vwidth = 0x4;
    kernel->den = 8;
  }
  else if (bits == 2) {
    kernel->vwidth = 0x2;
    kernel->den = 2;
  }
  else if (bits == 1) {
    kernel->vwidth = 0x1;
    kernel->den = 1;
  }
  else {
    free(kernel);
    return NULL;
  }

  int size = round_up_div(k * k * bits, 64);
  uint64_t *kernel_data = (uint64_t *)malloc(sizeof(uint64_t) * size);
  uint16_t *data = (uint16_t *)kernel_data;
  for (int i=0; i<size*4; i++) {
    // RAND_MAX assumed to be 32767
    data[i] = rand();
  }
  kernel->addr = (void *)kernel_data;

  return kernel;
}

void SetOutput(image_t *output_image) {

  int width = output_image->width;
  int height = output_image->height;
  uint8_t vwidth = output_image->vwidth;
  uint64_t *img_addr = output_image->addr;

  if (output_image->order == 0) {
    for (int i=0; i<height; i++) {
      for (int j=0; j<width; j++) {
        printf("  %d", get_main_value(img_addr, i * width + j, vwidth));
      }
      printf("\n");
    }
  }
  else {
    for (int i=0; i<height; i++) {
      for (int j=0; j<width; j++) {
        printf("  %d", get_main_value(img_addr, j * height + i, vwidth));
      }
      printf("\n");
    }
  }
}

void SetOutputKernel(kernel_t *output_kernel) {

  int k = output_kernel->size;
  uint8_t vwidth = output_kernel->vwidth;
  uint64_t *kernel_addr = output_kernel->addr;

  for (int i=0; i<k; i++) {
    for (int j=0; j<k; j++) {
      printf("  %d", get_kernel_value(kernel_addr, i * k + j, vwidth));
    }
    printf("\n");
  }
}

