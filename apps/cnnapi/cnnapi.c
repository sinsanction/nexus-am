#include "cnnapi.h"
#include <stdint.h>

inline int round_up_div(int a, int b) {
  int div = a / b;
  int rem = a % b;
  if (rem == 0) {
    return div;
  }
  else {
    return div + 1;
  }
}


// read low-bit data
inline uint8_t get_main_uint4(uint8_t *data, int i) {
  int j = i / 2;
  int r = i % 2;
  uint8_t data_i = (data[j] >> (r * 4)) & 0xf;
  return data_i;
}

inline uint8_t get_main_uint2(uint8_t *data, int i) {
  int j = i / 4;
  int r = i % 4;
  uint8_t data_i = (data[j] >> (r * 2)) & 0x3;
  return data_i;
}

inline int8_t get_kernel_int4(int8_t *kernel, int i) {
  int j = i / 2;
  int r = i % 2;
  int8_t kernel_i = (kernel[j] >> (r * 4)) & 0xf;
  if (kernel_i & 0x8) {
    kernel_i = kernel_i | 0xf0;
  }
  return kernel_i;
}

inline int8_t get_kernel_int2(int8_t *kernel, int i) {
  int j = i / 4;
  int r = i % 4;
  int8_t kernel_i = (kernel[j] >> (r * 2)) & 0x3;
  if (kernel_i & 0x2) {
    kernel_i = kernel_i | 0xfc;
  }
  return kernel_i;
}

inline int8_t get_kernel_int1(int8_t *kernel, int i) {
  int j = i / 8;
  int r = i % 8;
  int8_t kernel_i = (kernel[j] >> r) & 0x1;
  return kernel_i;
}

inline uint16_t get_main_value(uint64_t *img_addr, int i, uint8_t vwidth) {
  uint16_t res;
  if (vwidth == 0x10) {
    res = get_main_uint2((uint8_t *)img_addr, i);
  }
  else if (vwidth == 0x20) {
    res = get_main_uint4((uint8_t *)img_addr, i);
  }
  else if (vwidth == 0x40) {
    uint8_t *data = (uint8_t *)img_addr;
    res = data[i];
  }
  else { //vwidth == 0x80
    uint16_t *data = (uint16_t *)img_addr;
    res = data[i];
  }
  return res;
}

inline int8_t get_kernel_value(uint64_t *kernel_addr, int i, uint8_t vwidth) {
  int8_t res;
  if (vwidth == 0x1) {
    res = get_kernel_int1((int8_t *)kernel_addr, i);
  }
  else if (vwidth == 0x2) {
    res = get_kernel_int2((int8_t *)kernel_addr, i);
  }
  else if (vwidth == 0x4) {
    res = get_kernel_int4((int8_t *)kernel_addr, i);
  }
  else { //vwidth == 0x8
    int8_t *data = (int8_t *)kernel_addr;
    res = data[i];
  }
  return res;
}


// write low-bit data
inline void put_uint4(uint8_t *data, int i, uint8_t value) {
  int j = i / 2;
  int r = i % 2;

  if (r == 0) {
    data[j] = (data[j] & 0xf0) | value; 
  }
  else {
    data[j] = (data[j] & 0xf) | (value << 4); 
  }
}

inline void put_uint2(uint8_t *data, int i, uint8_t value) {
  int j = i / 4;
  int r = i % 4;

  if (r == 0) {
    data[j] = (data[j] & 0xfc) | value; 
  }
  else if (r == 1) {
    data[j] = (data[j] & 0xf3) | (value << 2); 
  }
  else if (r == 2) {
    data[j] = (data[j] & 0xcf) | (value << 4); 
  }
  else {
    data[j] = (data[j] & 0x3f) | (value << 6); 
  }
}

inline void put_uint1(uint8_t *data, int i, uint8_t value) {
  int j = i / 8;
  int r = i % 8;

  if (r == 0) {
    data[j] = (data[j] & 0xfe) | value; 
  }
  else if (r == 1) {
    data[j] = (data[j] & 0xfd) | (value << 1); 
  }
  else if (r == 2) {
    data[j] = (data[j] & 0xfb) | (value << 2); 
  }
  else if (r == 3) {
    data[j] = (data[j] & 0xf7) | (value << 3); 
  }
  else if (r == 4) {
    data[j] = (data[j] & 0xef) | (value << 4); 
  }
  else if (r == 5) {
    data[j] = (data[j] & 0xdf) | (value << 5); 
  }
  else if (r == 6) {
    data[j] = (data[j] & 0xbf) | (value << 6); 
  }
  else {
    data[j] = (data[j] & 0x7f) | (value << 7); 
  }
}

inline void put_main_value(uint64_t *img_addr, int i, uint8_t vwidth, uint16_t value) {
  if (vwidth == 0x10) {
    put_uint2((uint8_t *)img_addr, i, value);
  }
  else if (vwidth == 0x20) {
    put_uint4((uint8_t *)img_addr, i, value);
  }
  else if (vwidth == 0x40) {
    uint8_t *data = (uint8_t *)img_addr;
    data[i] = value;
  }
  else { //vwidth == 0x80
    uint16_t *data = (uint16_t *)img_addr;
    data[i] = value;
  }
}

