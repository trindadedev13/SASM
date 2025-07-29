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

/**
 * Setters of registers
 * A, B, C, D
 *
 * @param byte The register new value
 */
void sasm_regs_setA(byte);
void sasm_regs_setB(byte);
void sasm_regs_setC(byte);
void sasm_regs_setD(byte);

/**
 * Getters of registers
 * A, B, C, D
 *
 * Returns the register value(value)
 */
byte sasm_regs_getA();
byte sasm_regs_getB();
byte sasm_regs_getC();
byte sasm_regs_getD();

typedef enum { A, B, C, D } sasm_reg_type;

typedef struct {
  bool is_reg;
  sasm_reg_type type;
} sasm_regs_reginfo;

/**
 * Check if the valye is a register and returns it type.
 *
 * @param The value to checked
 *
 * see sasm_regs_reginfo
 */
sasm_regs_reginfo sasm_regs_get_reginfo(string);

/**
 * Assigns a new value to a reg
 *
 * @param The reg
 * @param The value(can be a reg too)
 */
void sasm_regs_mov(string, string);

/**
 * Adds a value to a register
 *
 * @param The register
 * @param The value 1
 * @param The value 2(optional)
 *
 * About value2:
 *
 * if value2 is not passed, so it will adds like:
 * regValue = regValue + value1
 * else
 * regValue = value1 + value2
 *
 * simple
 */
void sasm_regs_add(string, string, string);

/**
 * Subtract a value to a register
 *
 * @param The register
 * @param The value 1
 * @param The value 2(optional)
 *
 * About value2:
 *
 * if value2 is not passed, so it will subtracts like:
 * regValue = regValue - value1
 * else
 * regValue = value1 - value2
 *
 * simple
 */
void sasm_regs_sub(string, string, string);

#endif