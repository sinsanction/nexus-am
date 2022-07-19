#include "cnnapi.h"
#include "cnnapi_v2.h"

// IO
image_mp_t *RandomInitImage_MP_SC(uint32_t width, uint32_t height) {

  image_mp_t *img = (image_mp_t *)malloc(sizeof(image_mp_t));
  img->width = width;
  img->height = height;

  int vwidth_size = round_up_div(width, 8);
  img->vwidth = (uint8_t *)malloc(sizeof(uint64_t) * vwidth_size);
  img->addr = (void **)malloc(sizeof(void *) * width);

  for (int i=0; i<width; i++) {
    int random_bits = rand() % 4;
    int bits;
    if (random_bits == 3) {
      bits = 16;
      img->vwidth[i] = 0x80;
    }
    else if (random_bits == 2) {
      bits = 8;
      img->vwidth[i] = 0x40;
    }
    else if (random_bits == 1) {
      bits = 4;
      img->vwidth[i] = 0x20;
    }
    else {
      bits = 2;
      img->vwidth[i] = 0x10;
    }

    int size = round_up_div(height * bits, 64);
    uint64_t *img_data = (uint64_t *)malloc(sizeof(uint64_t) * size);
    uint16_t *data = (uint16_t *)img_data;
    for (int j=0; j<size*4; j++) {
      // RAND_MAX assumed to be 32767
      data[j] = rand();
    }
    img->addr[i] = (void *)img_data;
  }

  return img;
}

kernel_mp_t *RandomInitKernel_MP_SC(uint32_t k) {

  kernel_mp_t *kernel = (kernel_mp_t *)malloc(sizeof(kernel_mp_t));
  kernel->size = k;

  int vwidth_size = round_up_div(k, 8);
  kernel->vwidth = (uint8_t *)malloc(sizeof(uint64_t) * vwidth_size);
  int den_max = 0;

  for (int i=0; i<k; i++) {
    int random_bits = rand() % 4;
    int bits;
    if (random_bits == 3) {
      bits = 8;
      kernel->vwidth[i] = 0x8;
      den_max = 128;
    }
    else if (random_bits == 2) {
      bits = 4;
      kernel->vwidth[i] = 0x4;
      den_max = (den_max >= 8) ? den_max : 8;
    }
    else if (random_bits == 1) {
      bits = 2;
      kernel->vwidth[i] = 0x2;
      den_max = (den_max >= 2) ? den_max : 2;
    }
    else {
      bits = 1;
      kernel->vwidth[i] = 0x1;
      den_max = (den_max >= 1) ? den_max : 1;
    }

    int size = round_up_div(k * bits, 64);
    uint64_t *kernel_data = (uint64_t *)malloc(sizeof(uint64_t) * size);
    uint16_t *data = (uint16_t *)kernel_data;
    for (int j=0; j<size*4; j++) {
      // RAND_MAX assumed to be 32767
      data[j] = rand();
    }
    kernel->addr[i] = (void *)kernel_data;
  }
  kernel->den = den_max;

  return kernel;
}

image_mp_mc_t *RandomInitImage_MP(uint32_t width, uint32_t height, uint16_t channel) {
  
  image_mp_mc_t *img_mc = (image_mp_mc_t *)malloc(sizeof(image_mp_mc_t));
  img_mc->width = width;
  img_mc->height = height;
  img_mc->channel = channel;

  for (int i=0; i<channel; i++) {
    img_mc->img[i] = RandomInitImage_MP_SC(width, height);
  }

  return img_mc;
}

kernel_mp_mc_t *RandomInitKernel_MP(uint32_t k, uint16_t channel) {

  kernel_mp_mc_t *ker_mc = (kernel_mp_mc_t *)malloc(sizeof(kernel_mp_mc_t));
  ker_mc->size = k;
  ker_mc->channel = channel;

  for (int i=0; i<channel; i++) {
    ker_mc->ker[i] = RandomInitKernel_MP_SC(k);
  }

  return ker_mc;
}

fc_filter_mp_t *RandomInitFcFilter_MP(uint32_t width, uint32_t height) {

  fc_filter_mp_t *fc = (fc_filter_mp_t *)malloc(sizeof(fc_filter_mp_t));
  fc->width = width;
  fc->height = height;

  int vwidth_size = round_up_div(width, 8);
  fc->vwidth = (uint8_t *)malloc(sizeof(uint64_t) * vwidth_size);
  int den_max = 0;

  for (int i=0; i<width; i++) {
    int random_bits = rand() % 4;
    int bits;
    if (random_bits == 3) {
      bits = 8;
      fc->vwidth[i] = 0x8;
      den_max = 128;
    }
    else if (random_bits == 2) {
      bits = 4;
      fc->vwidth[i] = 0x4;
      den_max = (den_max >= 8) ? den_max : 8;
    }
    else if (random_bits == 1) {
      bits = 2;
      fc->vwidth[i] = 0x2;
      den_max = (den_max >= 2) ? den_max : 2;
    }
    else {
      bits = 1;
      fc->vwidth[i] = 0x1;
      den_max = (den_max >= 1) ? den_max : 1;
    }

    int size = round_up_div(height * bits, 64);
    uint64_t *fc_data = (uint64_t *)malloc(sizeof(uint64_t) * size);
    uint16_t *data = (uint16_t *)fc_data;
    for (int j=0; j<size*4; j++) {
      // RAND_MAX assumed to be 32767
      data[j] = rand();
    }
    fc->addr[i] = (void *)fc_data;
  }
  fc->den = den_max;

  return fc;
}

fc_filter_mp_t *RandomInitFcFilterArray_MP(uint32_t width, uint32_t height, int units) {

  fc_filter_mp_t *fc = (fc_filter_mp_t *)malloc(sizeof(fc_filter_mp_t) * units);

  for (int i=0; i<units; i++) {
    fc[i] = *RandomInitFcFilter_MP(width, height);
  }

  return fc;
}

void SetOutput_MP_SC(image_mp_t *output_image) {

  int width = output_image->width;
  int height = output_image->height;

  printf("width: %d, height: %d\n", width, height);

  for (int i=0; i<height; i++) {
    for (int j=0; j<width; j++) {
      printf("  %d", get_main_value((uint64_t *)(output_image->addr[j]), i, output_image->vwidth[j]));
    }
    printf("\n");
  }
}

void SetOutputKernel_MP_SC(kernel_mp_t *output_kernel) {

  int k = output_kernel->size;

  printf("k: %d\n", k);

  for (int i=0; i<k; i++) {
    for (int j=0; j<k; j++) {
      printf("  %d", get_kernel_value((uint64_t *)(output_kernel->addr[j]), i, output_kernel->vwidth[j]));
    }
    printf("\n");
  }
}

void SetOutput_MP(image_mp_mc_t *output_image) {

  printf("\nwidth: %d, height: %d, channel: %d\n", output_image->width, output_image->height, output_image->channel);

  for (int i=0; i<output_image->channel; i++) {
    printf("channel %d: \n", i);
    SetOutput_MP_SC(output_image->img[i]);
  }
}

void SetOutputKernel_MP(kernel_mp_mc_t *output_kernel) {

  printf("\nk: %d, channel: %d\n", output_kernel->size, output_kernel->channel);

  for (int i=0; i<output_kernel->channel; i++) {
    printf("channel %d: \n", i);
    SetOutputKernel_MP_SC(output_kernel->ker[i])
  }
}

void SetOutputFcFilter_MP(fc_filter_mp_t *output_fc_filter) {

  int width = output_fc_filter->width;
  int height = output_fc_filter->height;

  printf("\nwidth: %d, height: %d\n", width, height);

  for (int i=0; i<height; i++) {
    for (int j=0; j<width; j++) {
      printf("  %d", get_kernel_value((uint64_t *)(output_fc_filter->addr[j]), i, output_fc_filter->vwidth[j]));
    }
    printf("\n");
  }
}
