#ifndef __REGS_H__
#define __REGS_H__

#include "kcstd/types.h"

typedef struct {
  byte A;
  byte B;
  byte C;
  byte D;
} sasm_regs;

extern sasm_regs regs;

void sasm_regs_setA(byte);
void sasm_regs_setB(byte);
void sasm_regs_setC(byte);
void sasm_regs_setD(byte);

byte sasm_regs_getA();
byte sasm_regs_getB();
byte sasm_regs_getC();
byte sasm_regs_getD();

typedef enum { A, B, C, D } sasm_reg_type;

typedef struct {
  bool is_reg;
  sasm_reg_type type;
} sasm_regs_reginfo;

sasm_regs_reginfo sasm_regs_get_reginfo(string);

void sasm_regs_mov(string, string);
void sasm_regs_add(string, string, string);
void sasm_regs_sub(string, string, string);

#endif