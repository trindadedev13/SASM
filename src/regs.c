#include "sasm/regs.h"

#include "kcstd/io.h"
#include "kcstd/memory.h"
#include "kcstd/string.h"
#include "kcstd/types.h"

#include "sasm/cpu.h"

sasm_regs* sasm_regs_new() {
  sasm_regs* it = memory_alloc(sizeof(sasm_regs));
  it->A = 0x00;
  it->B = 0x00;
  it->C = 0x00;
  it->D = 0x00;
  it->SP = 0x00;
  return it;
}

void sasm_regs_delete(sasm_regs* self) {
  self->A = 0x00;
  self->B = 0x00;
  self->C = 0x00;
  self->D = 0x00;
  self->SP = 0x00;
  memory_free(self);
}

sasm_regs_reginfo sasm_regs_get_reginfo(string value) {
  if (str_equals(value, "A")) {
    return (sasm_regs_reginfo){true, REG_A};
  } else if (str_equals(value, "B")) {
    return (sasm_regs_reginfo){true, REG_B};
  } else if (str_equals(value, "C")) {
    return (sasm_regs_reginfo){true, REG_C};
  } else if (str_equals(value, "D")) {
    return (sasm_regs_reginfo){true, REG_D};
  } else if (str_equals(value, "SP")) {
    return (sasm_regs_reginfo){true, REG_SP};
  }
  return (sasm_regs_reginfo){false, -1};
}

// MOV dest_reg, value_reg
void sasm_regs_mov(string dest_reg, string value) {
  // map strings to enum
  sasm_reg_type dest_type;
  if (str_equals(dest_reg, "A"))
    dest_type = REG_A;
  else if (str_equals(dest_reg, "B"))
    dest_type = REG_B;
  else if (str_equals(dest_reg, "C"))
    dest_type = REG_C;
  else if (str_equals(dest_reg, "D"))
    dest_type = REG_D;
  else if (str_equals(dest_reg, "SP"))
    dest_type = REG_SP;
  else
    return;

  byte value_data;

  // try interpret value as register
  sasm_regs_reginfo reginfo = sasm_regs_get_reginfo(value);
  if (reginfo.is_reg) {
    value_data = *((&cpu->regs->A) + reginfo.type);  // acesso direto
  } else {
    if (str_starts_with(value, "0x") || str_starts_with(value, "0X")) {
      value_data = (byte)str_tol(value, NULL, 16);
    } else {
      value_data = (byte)str_tol(value, NULL, 10);
    }
  }

  *((&cpu->regs->A) + dest_type) = value_data;
}

void sasm_regs_add(string dest_reg, string value1, string value2) {
  bool v2_passed = value2 != NULL;

  sasm_reg_type dest_type;
  if (str_equals(dest_reg, "A"))
    dest_type = REG_A;
  else if (str_equals(dest_reg, "B"))
    dest_type = REG_B;
  else if (str_equals(dest_reg, "C"))
    dest_type = REG_C;
  else if (str_equals(dest_reg, "D"))
    dest_type = REG_D;
  else if (str_equals(dest_reg, "SP"))
    dest_type = REG_SP;
  else
    return;

  byte val1 = 0, val2 = 0;

  {
    string v1 = (v2_passed) ? value1 : dest_reg;
    sasm_regs_reginfo reginfo1 = sasm_regs_get_reginfo(v1);
    if (reginfo1.is_reg) {
      val1 = *((&cpu->regs->A) + reginfo1.type);
    } else if (str_starts_with(v1, "0x") || str_starts_with(v1, "0X")) {
      val1 = (byte)str_tol(v1, NULL, 16);
    } else {
      val1 = (byte)str_tol(v1, NULL, 10);
    }
  }

  {
    string v2 = (v2_passed) ? value2 : value1;
    sasm_regs_reginfo reginfo2 = sasm_regs_get_reginfo(v2);
    if (reginfo2.is_reg) {
      val2 = *((&cpu->regs->A) + reginfo2.type);
    } else if (str_starts_with(v2, "0x") || str_starts_with(v2, "0X")) {
      val2 = (byte)str_tol(v2, NULL, 16);
    } else {
      val2 = (byte)str_tol(v2, NULL, 10);
    }
  }

  *((&cpu->regs->A) + dest_type) = val1 + val2;
}

void sasm_regs_sub(string dest_reg, string value1, string value2) {
  bool v2_passed = value2 != NULL;

  sasm_reg_type dest_type;
  if (str_equals(dest_reg, "A"))
    dest_type = REG_A;
  else if (str_equals(dest_reg, "B"))
    dest_type = REG_B;
  else if (str_equals(dest_reg, "C"))
    dest_type = REG_C;
  else if (str_equals(dest_reg, "D"))
    dest_type = REG_D;
  else if (str_equals(dest_reg, "SP"))
    dest_type = REG_SP;
  else
    return;

  byte val1 = 0, val2 = 0;

  {
    string v1 = (v2_passed) ? value1 : dest_reg;
    sasm_regs_reginfo reginfo1 = sasm_regs_get_reginfo(v1);
    if (reginfo1.is_reg) {
      val1 = *((&cpu->regs->A) + reginfo1.type);
    } else if (str_starts_with(v1, "0x") || str_starts_with(v1, "0X")) {
      val1 = (byte)str_tol(v1, NULL, 16);
    } else {
      val1 = (byte)str_tol(v1, NULL, 10);
    }
  }

  {
    string v2 = (v2_passed) ? value2 : value1;
    sasm_regs_reginfo reginfo2 = sasm_regs_get_reginfo(v2);
    if (reginfo2.is_reg) {
      val2 = *((&cpu->regs->A) + reginfo2.type);
    } else if (str_starts_with(v2, "0x") || str_starts_with(v2, "0X")) {
      val2 = (byte)str_tol(v2, NULL, 16);
    } else {
      val2 = (byte)str_tol(v2, NULL, 10);
    }
  }

  *((&cpu->regs->A) + dest_type) = val1 - val2;
}