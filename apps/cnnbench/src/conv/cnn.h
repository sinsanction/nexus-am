#ifndef __CNN_H__
#define __CNN_H__

#ifdef __cplusplus
extern "C" {
#endif

inline void LoadV_Width(uint64_t addr, int imm) {
  __asm__ volatile(
    ".set op, 0xb\n"
    ".set rd, 0x0\n"
    ".set func3, 0x0\n"
    ".byte op | ((rd & 0x1) << 7),\
     ((rd >> 1) & 0xf) | (func3 << 4) | ((r%[addr] & 0x1) << 7),\
     ((r%[addr] >> 1) & 0xf) | ((%[imm] & 0xf) << 4),\
     ((%[imm] >> 4) & 0xff)"
    : :[addr]"r"(addr), [imm]"i"(imm)
  );
}

inline void LoadV_D(uint64_t addr, uint8_t k, uint8_t vtag, uint8_t vec_addr) {
  __asm__ volatile(
    ".set op, 0xb\n"
    ".set func3, 0x2\n"
    ".set opf4, 0x00\n"
    ".byte op | ((%[vec_addr] & 0x1) << 7),\
     ((%[vec_addr] >> 1) & 0xf) | (func3 << 4) | ((r%[addr] & 0x1) << 7),\
     ((r%[addr] >> 1) & 0xf) | ((%[k] & 0xf) << 4),\
     (%[vtag] & 0x7) | opf4"
    : :[addr]"r"(addr), [k]"i"(k), [vtag]"i"(vtag), [vec_addr]"i"(vec_addr)
  );
}

inline void LoadV_P(uint64_t addr, uint8_t k, uint8_t vtag) {
  __asm__ volatile(
    ".set op, 0xb\n"
    ".set rd, 0x0\n"
    ".set func3, 0x3\n"
    ".set opf4, 0x00\n"
    ".byte op | ((rd & 0x1) << 7),\
     ((rd >> 1) & 0xf) | (func3 << 4) | ((r%[addr] & 0x1) << 7),\
     ((r%[addr] >> 1) & 0xf) | ((%[k] & 0xf) << 4),\
     (%[vtag] & 0x7) | opf4"
    : :[addr]"r"(addr), [k]"i"(k), [vtag]"i"(vtag)
  );
}

inline uint16_t Conv(uint8_t k) {
  register uint16_t res;
  __asm__ volatile(
    ".set op, 0xb\n"
    ".set func3, 0x4\n"
    ".set rs1, 0x0\n"
    ".set opf4, 0x00\n"
    ".byte op | ((r%[res] & 0x1) << 7),\
     ((r%[res] >> 1) & 0xf) | (func3 << 4) | ((rs1 & 0x1) << 7),\
     ((rs1 >> 1) & 0xf) | ((%[k] & 0xf) << 4),\
     opf4"
    :[res]"=r"(res)
    :[k]"i"(k), "[res]"(res)
  );
  return res;
}

inline uint16_t Pool(uint8_t k, uint8_t algorithm) {
  register uint16_t res;
  __asm__ volatile(
    ".set op, 0xb\n"
    ".set func3, 0x6\n"
    ".set opf4, 0x00\n"
    ".byte op | ((r%[res] & 0x1) << 7),\
     ((r%[res] >> 1) & 0xf) | (func3 << 4) | ((%[algorithm] & 0x1) << 7),\
     ((%[algorithm] >> 1) & 0xf) | ((%[k] & 0xf) << 4),\
     opf4"
    :[res]"=r"(res)
    :[k]"i"(k), [algorithm]"i"(algorithm), "[res]"(res)
  );
  return res;
}

inline uint64_t Act(uint64_t data, uint64_t zero) {
  register uint64_t res;
  __asm__ volatile(
    ".set op, 0xb\n"
    ".set func3, 0x7\n"
    ".set opf4, 0x00\n"
    ".byte op | ((r%[res] & 0x1) << 7),\
     ((r%[res] >> 1) & 0xf) | (func3 << 4) | ((r%[data] & 0x1) << 7),\
     ((r%[data] >> 1) & 0xf) | ((r%[zero] & 0xf) << 4),\
     ((r%[zero] >> 4) & 0x1) | (opf4 << 1)"
    :[res]"=r"(res)
    :[data]"r"(data), [zero]"r"(zero), "[res]"(res)
  );
  return res;
}

#ifdef __cplusplus
}
#endif

#endif
