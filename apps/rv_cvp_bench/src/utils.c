#include "utils.h"


// read low-bit data
static int8_t get_data_int2(int8_t *data, int i) {
  int j = i / 4;
  int r = i % 4;
  int8_t data_i = (data[j] >> (r * 2)) & 0x3;
  if (data_i & 0x2) {
    data_i = data_i | 0xfc;
  }
  return data_i;
}

static int8_t get_data_int4(int8_t *data, int i) {
  int j = i / 2;
  int r = i % 2;
  int8_t data_i = (data[j] >> (r * 4)) & 0xf;
  if (data_i & 0x8) {
    data_i = data_i | 0xf0;
  }
  return data_i;
}

int16_t GetRawData(uint64_t *addr, int i, uint8_t sew) {
  int16_t res;

  if (sew == SEW_2BIT) {
    res = get_data_int2((int8_t *)addr, i);
  }
  else if (sew == SEW_4BIT) {
    res = get_data_int4((int8_t *)addr, i);
  }
  else if (sew == SEW_8BIT) {
    int8_t *data = (int8_t *)addr;
    res = data[i];
  }
  else { //sew == SEW_16BIT
    int16_t *data = (int16_t *)addr;
    res = data[i];
  }
  return res;
}

// write low-bit data
static void put_data_int2(int8_t *data, int i, int8_t value) {
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

static void put_data_int4(int8_t *data, int i, int8_t value) {
  int j = i / 2;
  int r = i % 2;

  if (r == 0) {
    data[j] = (data[j] & 0xf0) | value; 
  }
  else {
    data[j] = (data[j] & 0xf) | (value << 4); 
  }
}

void PutRawData(uint64_t *addr, int i, uint8_t sew, int16_t value) {
  if (sew == SEW_2BIT) {
    put_data_int2((int8_t *)addr, i, value);
  }
  else if (sew == SEW_4BIT) {
    put_data_int4((int8_t *)addr, i, value);
  }
  else if (sew == SEW_8BIT) {
    int8_t *data = (int8_t *)addr;
    data[i] = value;
  }
  else { //sew == SEW_16BIT
    int16_t *data = (int16_t *)addr;
    data[i] = value;
  }
}

// others
inline int RoundUpDiv(int a, int b) {
  int div = a / b;
  int rem = a % b;
  return (rem == 0) ? div : div + 1;
}
