#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define OPCODE 0xb
#define CONV 0x0
#define POOL 0x2
#define POOL_M 0x3
#define ACT 0x4
#define ADD 0x5

#define ALG_POOL_MAX 0x1
#define ALG_POOL_AVG 0x2
#define ALG_ACT_RELU 0x0
#define ALG_ACT_RELU6 0x1
#define NO_FUSION 0x0
#define FUSION_RELU 0x1

#define SEW_2BIT 0x0
#define SEW_4BIT 0x1
#define SEW_8BIT 0x2
#define SEW_16BIT 0x3

uint32_t Conv(uint8_t sew, uint8_t src_reg, uint8_t ker_reg, uint8_t dst_reg) {
  uint8_t op = OPCODE;
  uint8_t func3 = CONV;
  uint8_t rs1 = src_reg;
  uint8_t rs2 = ker_reg;
  uint8_t rd = dst_reg;
  uint8_t opf4 = 0x0;

  uint8_t ins0 = op | ((rd & 0x1) << 7);
  uint8_t ins1 = ((rd >> 1) & 0xf) | (func3 << 4) | ((rs1 & 0x1) << 7);
  uint8_t ins2 = ((rs1 >> 1) & 0xf) | ((rs2 & 0xf) << 4);
  uint8_t ins3 = ((rs2 >> 4) & 0x1) | (sew << 1) | (opf4 << 4);

  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}

uint32_t Pool(uint8_t sew, uint8_t src_reg, uint8_t algorithm, uint8_t dst_reg) {
  uint8_t op = OPCODE;
  uint8_t func3 = POOL;
  uint8_t rs1 = src_reg;
  uint8_t rs2 = algorithm;
  uint8_t rd = dst_reg;
  uint8_t opf4 = 0x0;

  uint8_t ins0 = op | ((rd & 0x1) << 7);
  uint8_t ins1 = ((rd >> 1) & 0xf) | (func3 << 4) | ((rs1 & 0x1) << 7);
  uint8_t ins2 = ((rs1 >> 1) & 0xf) | ((rs2 & 0xf) << 4);
  uint8_t ins3 = ((rs2 >> 4) & 0x1) | (sew << 1) | (opf4 << 4);

  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}

uint32_t Pool_M(uint8_t sew, uint8_t src_reg, uint8_t algorithm, uint8_t dst_reg, uint8_t k) {
  uint8_t op = OPCODE;
  uint8_t func3 = POOL_M;
  uint8_t rs1 = src_reg;
  uint8_t rs2 = algorithm;
  uint8_t rd = dst_reg;
  uint8_t opf4 = k;

  uint8_t ins0 = op | ((rd & 0x1) << 7);
  uint8_t ins1 = ((rd >> 1) & 0xf) | (func3 << 4) | ((rs1 & 0x1) << 7);
  uint8_t ins2 = ((rs1 >> 1) & 0xf) | ((rs2 & 0xf) << 4);
  uint8_t ins3 = ((rs2 >> 4) & 0x1) | (sew << 1) | (opf4 << 4);

  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}

uint32_t Act(uint8_t sew, uint8_t src_reg, uint8_t zero_reg, uint8_t dst_reg, uint8_t algorithm) {
  uint8_t op = OPCODE;
  uint8_t func3 = ACT;
  uint8_t rs1 = src_reg;
  uint8_t rs2 = zero_reg;
  uint8_t rd = dst_reg;
  uint8_t opf4 = algorithm;
  
  uint8_t ins0 = op | ((rd & 0x1) << 7);
  uint8_t ins1 = ((rd >> 1) & 0xf) | (func3 << 4) | ((rs1 & 0x1) << 7);
  uint8_t ins2 = ((rs1 >> 1) & 0xf) | ((rs2 & 0xf) << 4);
  uint8_t ins3 = ((rs2 >> 4) & 0x1) | (sew << 1) | (opf4 << 4);

  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}

uint32_t Add(uint8_t sew, uint8_t src_a_reg, uint8_t src_b_reg, uint8_t dst_reg, uint8_t relu) {
  uint8_t op = OPCODE;
  uint8_t func3 = ADD;
  uint8_t rs1 = src_a_reg;
  uint8_t rs2 = src_b_reg;
  uint8_t rd = dst_reg;
  uint8_t opf4 = relu;
  
  uint8_t ins0 = op | ((rd & 0x1) << 7);
  uint8_t ins1 = ((rd >> 1) & 0xf) | (func3 << 4) | ((rs1 & 0x1) << 7);
  uint8_t ins2 = ((rs1 >> 1) & 0xf) | ((rs2 & 0xf) << 4);
  uint8_t ins3 = ((rs2 >> 4) & 0x1) | (sew << 1) | (opf4 << 4);

  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}


#define REG_a0 (10)
#define REG_a1 (11)
#define REG_a2 (12)
#define REG_a3 (13)

void conv_generator() {
  uint8_t src_reg = REG_a0;
  uint8_t ker_reg = REG_a1;
  uint8_t dst_reg = REG_a2;
  uint8_t sew;
  uint32_t ins;

  printf("conv: src_reg=a0, ker_reg=a1, dst_reg=a2: \n");
  for (sew=0; sew<=3; sew++) {
    ins = Conv(sew, src_reg, ker_reg, dst_reg);
    printf("  sew=%.2d : %#.8x\n", sew, ins);
  }
  printf("\n");
}

void pool_generator() {
  uint8_t src_reg = REG_a0;
  uint8_t dst_reg = REG_a1;
  uint8_t algorithm;
  uint8_t sew;
  uint32_t ins;

  //max pool
  printf("pool.max: src_reg=a0, dst_reg=a1: \n");
  algorithm = ALG_POOL_MAX;
  for (sew=0; sew<=3; sew++) {
    ins = Pool(sew, src_reg, algorithm, dst_reg);
    printf("  sew=%.2d : %#.8x\n", sew, ins);
  }
  printf("\n");

  //avg pool
  printf("pool.avg: src_reg=a0, dst_reg=a1: \n");
  algorithm = ALG_POOL_AVG;
  for (sew=0; sew<=3; sew++) {
    ins = Pool(sew, src_reg, algorithm, dst_reg);
    printf("  sew=%.2d : %#.8x\n", sew, ins);
  }
  printf("\n");
}

void pool_m_generator() {
  uint8_t src_reg = REG_a0;
  uint8_t dst_reg = REG_a1;
  uint8_t algorithm;
  uint8_t window = 2;
  uint8_t sew;
  uint32_t ins;

  //max pool.m
  printf("pool.m.max: src_reg=a0, dst_reg=a1: \n");
  algorithm = ALG_POOL_MAX;
  for (sew=0; sew<=3; sew++) {
    ins = Pool_M(sew, src_reg, algorithm, dst_reg, window);
    printf("  sew=%.2d : %#.8x\n", sew, ins);
  }
  printf("\n");

  //avg pool.m
  printf("pool.m.avg: src_reg=a0, dst_reg=a1: \n");
  algorithm = ALG_POOL_AVG;
  for (sew=0; sew<=3; sew++) {
    ins = Pool_M(sew, src_reg, algorithm, dst_reg, window);
    printf("  sew=%.2d : %#.8x\n", sew, ins);
  }
  printf("\n");
}

void act_generator() {
  uint8_t src_reg = REG_a0;
  uint8_t zero_reg = REG_a1;
  uint8_t dst_reg = REG_a2;
  uint8_t algorithm;
  uint8_t sew;
  uint32_t ins;

  //relu
  printf("act.relu: src_reg=a0, zero_reg=a1, dst_reg=a2: \n");
  algorithm = ALG_ACT_RELU;
  for (sew=0; sew<=3; sew++) {
    ins = Act(sew, src_reg, zero_reg, dst_reg, algorithm);
    printf("  sew=%.2d : %#.8x\n", sew, ins);
  }
  printf("\n");

  //relu6
  printf("act.relu6: src_reg=a0, zero_reg=a1, dst_reg=a2: \n");
  algorithm = ALG_ACT_RELU6;
  for (sew=0; sew<=3; sew++) {
    ins = Act(sew, src_reg, zero_reg, dst_reg, algorithm);
    printf("  sew=%.2d : %#.8x\n", sew, ins);
  }
  printf("\n");
}

void add_generator() {
  uint8_t src_a_reg = REG_a0;
  uint8_t src_b_reg = REG_a1;
  uint8_t dst_reg = REG_a2;
  uint8_t fusion;
  uint8_t sew;
  uint32_t ins;

  //no fusion
  printf("add.n: src_a_reg=a0, src_b_reg=a1, dst_reg=a2: \n");
  fusion = NO_FUSION;
  for (sew=0; sew<=3; sew++) {
    ins = Add(sew, src_a_reg, src_b_reg, dst_reg, fusion);
    printf("  sew=%.2d : %#.8x\n", sew, ins);
  }
  printf("\n");

  //fusion relu
  printf("add.relu: src_a_reg=a0, src_b_reg=a1, dst_reg=a2: \n");
  fusion = FUSION_RELU;
  for (sew=0; sew<=3; sew++) {
    ins = Add(sew, src_a_reg, src_b_reg, dst_reg, fusion);
    printf("  sew=%.2d : %#.8x\n", sew, ins);
  }
  printf("\n");
}

int main() {
  // conv
  conv_generator();

  // pool
  pool_generator();

  // pool.m
  pool_m_generator();

  // act
  act_generator();

  // add
  add_generator();

  return 0;
}