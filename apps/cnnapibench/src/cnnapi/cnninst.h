#ifndef __CNNINST_H__
#define __CNNINST_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

inline void LoadV_Width(uint64_t addr);

inline void LoadV_D_Main(uint64_t addr, uint8_t k, uint8_t vec_addr, uint8_t vtag);

inline void LoadV_D_Kernel(uint64_t addr, uint8_t k, uint8_t vec_addr, uint8_t vtag);

inline void LoadV_P(uint64_t addr, uint8_t k, uint8_t vtag);

inline uint32_t Conv(uint8_t k);

inline uint16_t Pool_Max(uint8_t k);

inline uint16_t Pool_Avg(uint8_t k);

inline uint64_t Act(uint64_t data, uint64_t zero);

#ifdef __cplusplus
}
#endif

#endif
