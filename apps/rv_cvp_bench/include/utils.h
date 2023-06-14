#ifndef __CVP_UTILS_H__
#define __CVP_UTILS_H__

#include <stdint.h>

#define SEW_2BIT 0x0
#define SEW_4BIT 0x1
#define SEW_8BIT 0x2
#define SEW_16BIT 0x3

#ifdef __cplusplus
extern "C" {
#endif

// read low-bit data
int16_t GetRawData(uint64_t *addr, int i, uint8_t sew);

// write low-bit data
void PutRawData(uint64_t *addr, int i, uint8_t sew, int16_t value);

// others
int RoundUpDiv(int a, int b);

#ifdef __cplusplus
}
#endif

#endif
