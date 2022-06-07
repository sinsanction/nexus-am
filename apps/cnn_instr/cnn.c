#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t LoadV_Width(uint8_t reg_addr) {
  uint8_t op = 0xb;
  uint8_t rd = 0x0;
  uint8_t func3=0x0;
  uint16_t imm = 0x0;

  uint8_t ins0 = op | ((rd & 0x1) << 7);
  uint8_t ins1 = ((rd >> 1) & 0xf) | (func3 << 4) | ((reg_addr & 0x1) << 7);
  uint8_t ins2 = ((reg_addr >> 1) & 0xf) | ((imm & 0xf) << 4);
  uint8_t ins3 = (imm >> 4) & 0xff;
  
  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}

uint32_t LoadV_D(uint8_t reg_addr, uint8_t k, uint8_t vtag, uint8_t vec_addr) {
  uint8_t op = 0xb;
  uint8_t func3 = 0x2;
  uint8_t opf4 = 0x0;

  uint8_t ins0 = op | ((vec_addr & 0x1) << 7);
  uint8_t ins1 = ((vec_addr >> 1) & 0xf) | (func3 << 4) | ((reg_addr & 0x1) << 7);
  uint8_t ins2 = ((reg_addr >> 1) & 0xf) | ((k & 0xf) << 4);
  uint8_t ins3 = (vtag & 0x7) | opf4;

  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}

uint32_t LoadV_P(uint8_t reg_addr, uint8_t k, uint8_t vtag) {
  uint8_t op = 0xb;
  uint8_t rd = 0x0;
  uint8_t func3 = 0x3;
  uint8_t opf4 = 0x0;

  uint8_t ins0 = op | ((rd & 0x1) << 7);
  uint8_t ins1 = ((rd >> 1) & 0xf) | (func3 << 4) | ((reg_addr & 0x1) << 7);
  uint8_t ins2 = ((reg_addr >> 1) & 0xf) | ((k & 0xf) << 4);
  uint8_t ins3 = (vtag & 0x7) | opf4;

  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}

uint32_t Conv(uint8_t k, uint8_t reg_res) {
  uint8_t op = 0xb;
  uint8_t func3 = 0x4;
  uint8_t rs1 = 0x0;
  uint8_t opf4 = 0x0;

  uint8_t ins0 = op | ((reg_res & 0x1) << 7);
  uint8_t ins1 = ((reg_res >> 1) & 0xf) | (func3 << 4) | ((rs1 & 0x1) << 7);
  uint8_t ins2 = ((rs1 >> 1) & 0xf) | ((k & 0xf) << 4);
  uint8_t ins3 = opf4;

  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}

uint32_t Pool(uint8_t k, uint8_t algorithm, uint8_t reg_res) {
  uint8_t op = 0xb;
  uint8_t func3 = 0x6;
  uint8_t opf4 = 0x0;

  uint8_t ins0 = op | ((reg_res & 0x1) << 7);
  uint8_t ins1 = ((reg_res >> 1) & 0xf) | (func3 << 4) | ((algorithm & 0x1) << 7);
  uint8_t ins2 = ((algorithm >> 1) & 0xf) | ((k & 0xf) << 4);
  uint8_t ins3 = opf4;

  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}

uint32_t Act(uint8_t reg_data, uint8_t reg_zero, uint8_t reg_res) {
  uint8_t op = 0xb;
  uint8_t func3 = 0x7;
  uint8_t opf4 = 0x0;
  
  uint8_t ins0 = op | ((reg_res & 0x1) << 7);
  uint8_t ins1 = ((reg_res >> 1) & 0xf) | (func3 << 4) | ((reg_data & 0x1) << 7);
  uint8_t ins2 = ((reg_data >> 1) & 0xf) | ((reg_zero & 0xf) << 4);
  uint8_t ins3 = ((reg_zero >> 4) & 0x1) | (opf4 << 1);

  uint32_t res = ins0 | (((uint32_t)ins1) << 8) | (((uint32_t)ins2) << 16) | (((uint32_t)ins3) << 24);
  return res;
}

void conv_generator() {
  uint8_t reg_res = 10;  //a0
  uint8_t k;
  uint32_t ins;

  for (k=1; k<=5; k++) {
    ins = Conv(k, reg_res);
    printf("conv: res_reg=a0, k=%d : %#.8x\n", k, ins);
  }
  printf("\n");
}

void pool_generator() {
  uint8_t reg_res = 10;  //a0
  uint8_t algorithm;
  uint8_t k;
  uint32_t ins;

  //max pool
  algorithm = 0x1;
  for (k=1; k<=5; k++) {
    ins = Pool(k, algorithm, reg_res);
    printf("pool-max: res_reg=a0, k=%d : %#.8x\n", k, ins);
  }

  //avg pool
  algorithm = 0x2;
  for (k=1; k<=5; k++) {
    ins = Pool(k, algorithm, reg_res);
    printf("pool-avg: res_reg=a0, k=%d : %#.8x\n", k, ins);
  }
  printf("\n");
}

void act_generator() {
  uint8_t reg_data = 10;  //a0
  uint8_t reg_zero = 11;  //a1
  uint8_t reg_res = 12;   //a2
  uint32_t ins;

  ins = Act(reg_data, reg_zero, reg_res);
  printf("act: res_reg=a2 : %#.8x\n", ins);
  printf("\n");
}

void loadvw_generator() {
  uint8_t reg_addr = 10;  //a0
  uint32_t ins;

  ins = LoadV_Width(reg_addr);
  printf("loadv-w: addr_reg=a0 : %#.8x\n", ins);
  printf("\n");
}

void loadvd_generator() {
  uint8_t reg_addr = 10;  //a0
  uint8_t k,j;
  uint8_t vtag = 0;
  uint8_t vec_addr;
  uint32_t ins;

  //main
  for (k=1; k<=5; k++) {
    for (j=0; j<k; j++) {
      vec_addr = j;
      ins = LoadV_D(reg_addr, k, vtag, vec_addr);
      printf("loadv-d-main: addr_reg=a0, k=%d, vaddr=%#.2x, vtag=0 : %#.8x\n", k, vec_addr, ins);
    }
  }
  printf("\n");

  //kernel
  for (k=1; k<=5; k++) {
    for (j=0; j<k; j++) {
      vec_addr = 0x10 | j;
      ins = LoadV_D(reg_addr, k, vtag, vec_addr);
      printf("loadv-d-kernel: addr_reg=a0, k=%d, vaddr=%#.2x, vtag=0 : %#.8x\n", k, vec_addr, ins);
    }
  }
  printf("\n");
}

void loadvp_generator() {
  uint8_t reg_addr = 10;  //a0
  uint8_t k;
  uint8_t vtag = 0;
  uint32_t ins;

  for (k=1; k<=5; k++) {
    ins = LoadV_P(reg_addr, k, vtag);
    printf("loadv-p: addr_reg=a0, k=%d, vtag=0 : %#.8x\n", k, ins);
  }
  printf("\n");
}

int main() {
  // conv
  conv_generator();

  // pool
  pool_generator();

  // act
  act_generator();

  // loadv-w
  loadvw_generator();

  // loadv-d
  loadvd_generator();

  // loadv-p
  loadvp_generator();

  return 0;
}
