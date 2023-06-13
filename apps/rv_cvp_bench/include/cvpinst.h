#ifndef __CVPINST_H__
#define __CVPINST_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t Conv(uint64_t src, uint64_t ker, int32_t last, int sew);

int16_t Pool_Max(uint64_t src, int16_t last, int sew);
int32_t Pool_Avg(uint64_t src, int32_t last, int sew);

int16_t Pool_M_Max(uint64_t src, int sew);
int16_t Pool_M_Avg(uint64_t src, int sew);

uint64_t Act_Relu(uint64_t src, uint64_t zero, int sew);
uint64_t Act_Relu6(uint64_t src, uint64_t zero, int sew);

uint64_t Add_N(uint64_t src_a, uint64_t src_b, uint64_t zero, int sew);
uint64_t Add_Relu(uint64_t src_a, uint64_t src_b, uint64_t zero, int sew);

#ifdef __cplusplus
}
#endif

#endif
