#include "sasm/regs.h"

#include "kcstd/io.h"
#include "kcstd/string.h"
#include "kcstd/types.h"

sasm_regs regs = {0x00, 0x00, 0x00, 0x00};

// SET

void sasm_regs_setA(byte bytes) {
  regs.A = bytes;
}

void sasm_regs_setB(byte bytes) {
  regs.B = bytes;
}

void sasm_regs_setC(byte bytes) {
  regs.C = bytes;
}

void sasm_regs_setD(byte bytes) {
  regs.D = bytes;
}

// BET

byte sasm_regs_getA() {
  return regs.A;
}

byte sasm_regs_getB() {
  return regs.B;
}

byte sasm_regs_getC() {
  return regs.C;
}

byte sasm_regs_getD() {
  return regs.D;
}

sasm_regs_reginfo sasm_regs_get_reginfo(string value) {
  if (str_equals(value, "A")) {
    return (sasm_regs_reginfo){true, A};
  } else if (str_equals(value, "B")) {
    return (sasm_regs_reginfo){true, B};
  } else if (str_equals(value, "C")) {
    return (sasm_regs_reginfo){true, C};
  } else if (str_equals(value, "D")) {
    return (sasm_regs_reginfo){true, D};
  }
  return (sasm_regs_reginfo){false, -1};
}

// MOV dest_reg, value_reg
void sasm_regs_mov(string dest_reg, string value) {
  // map strings to enum
  sasm_reg_type dest_type;
  if (str_equals(dest_reg, "A"))
    dest_type = A;
  else if (str_equals(dest_reg, "B"))
    dest_type = B;
  else if (str_equals(dest_reg, "C"))
    dest_type = C;
  else if (str_equals(dest_reg, "D"))
    dest_type = D;
  else
    return;  // invalid dest register

  // getter & setters
  byte (*get_funcs[])() = {sasm_regs_getA, sasm_regs_getB, sasm_regs_getC,
                           sasm_regs_getD};
  void (*set_funcs[])(byte) = {sasm_regs_setA, sasm_regs_setB, sasm_regs_setC,
                               sasm_regs_setD};

  byte value_data;

  // try interpret value as register
  sasm_regs_reginfo reginfo = sasm_regs_get_reginfo(value);
  if (reginfo.is_reg) {
    value_data = get_funcs[reginfo.type]();
  } else {
    // try interpret as literal (decimal or hexadecimal)
    if (str_starts_with(value, "0x") || str_starts_with(value, "0X")) {
      value_data = (byte)str_tol(value, NULL, 16);  // hexadecimal
    } else {
      value_data = (byte)str_tol(value, NULL, 10);  // decimal
    }
  }

  // assign value to dest reg
  set_funcs[dest_type](value_data);
}

// ADD dest_reg, value1, [value2]
void sasm_regs_add(string dest_reg, string value1, string value2) {
  bool v2_passed = value2 != null;

  // map strings to enum
  sasm_reg_type dest_type;
  if (str_equals(dest_reg, "A"))
    dest_type = A;
  else if (str_equals(dest_reg, "B"))
    dest_type = B;
  else if (str_equals(dest_reg, "C"))
    dest_type = C;
  else if (str_equals(dest_reg, "D"))
    dest_type = D;
  else
    return;  // invalid dest register

  // getter & setters
  byte (*get_funcs[])() = {sasm_regs_getA, sasm_regs_getB, sasm_regs_getC,
                           sasm_regs_getD};
  void (*set_funcs[])(byte) = {sasm_regs_setA, sasm_regs_setB, sasm_regs_setC,
                               sasm_regs_setD};

  byte val1 = 0;
  byte val2 = 0;

  // interpret value1
  {
    string v1 = (v2_passed) ? value1 : dest_reg;
    sasm_regs_reginfo reginfo1 = sasm_regs_get_reginfo(v1);
    if (reginfo1.is_reg) {
      val1 = get_funcs[reginfo1.type]();
    } else {
      if (str_starts_with(v1, "0x") || str_starts_with(v1, "0X")) {
        val1 = (byte)str_tol(v1, NULL, 16);
      } else {
        val1 = (byte)str_tol(v1, NULL, 10);
      }
    }
  }

  // interpret value2
  {
    string v2 = (v2_passed) ? value2 : value1;
    sasm_regs_reginfo reginfo2 = sasm_regs_get_reginfo(v2);
    if (reginfo2.is_reg) {
      val2 = get_funcs[reginfo2.type]();
    } else {
      if (str_starts_with(v2, "0x") || str_starts_with(v2, "0X")) {
        val2 = (byte)str_tol(v2, NULL, 16);
      } else {
        val2 = (byte)str_tol(v2, NULL, 10);
      }
    }
  }

  // calc result (with overflow wraparound)
  byte result = val1 + val2;

  // asign to destination register
  set_funcs[dest_type](result);
}

// SUB dest_reg, value1, [value2]
void sasm_regs_sub(string dest_reg, string value1, string value2) {
  bool v2_passed = value2 != null;

  // map strings to enum
  sasm_reg_type dest_type;
  if (str_equals(dest_reg, "A"))
    dest_type = A;
  else if (str_equals(dest_reg, "B"))
    dest_type = B;
  else if (str_equals(dest_reg, "C"))
    dest_type = C;
  else if (str_equals(dest_reg, "D"))
    dest_type = D;
  else
    return;  // invalid dest register

  // getter & setters
  byte (*get_funcs[])() = {sasm_regs_getA, sasm_regs_getB, sasm_regs_getC,
                           sasm_regs_getD};
  void (*set_funcs[])(byte) = {sasm_regs_setA, sasm_regs_setB, sasm_regs_setC,
                               sasm_regs_setD};

  byte val1 = 0;
  byte val2 = 0;

  // interpret value1
  {
    string v1 = (v2_passed) ? value1 : dest_reg;
    sasm_regs_reginfo reginfo1 = sasm_regs_get_reginfo(v1);
    if (reginfo1.is_reg) {
      val1 = get_funcs[reginfo1.type]();
    } else {
      if (str_starts_with(v1, "0x") || str_starts_with(v1, "0X")) {
        val1 = (byte)str_tol(v1, NULL, 16);
      } else {
        val1 = (byte)str_tol(v1, NULL, 10);
      }
    }
  }

  // interpret value2
  {
    string v2 = (v2_passed) ? value2 : value1;
    sasm_regs_reginfo reginfo2 = sasm_regs_get_reginfo(v2);
    if (reginfo2.is_reg) {
      val2 = get_funcs[reginfo2.type]();
    } else {
      if (str_starts_with(v2, "0x") || str_starts_with(v2, "0X")) {
        val2 = (byte)str_tol(v2, NULL, 16);
      } else {
        val2 = (byte)str_tol(v2, NULL, 10);
      }
    }
  }

  // calculate result with 8-bit wraparound (overflow)
  byte result = val1 - val2;

  // assign to destination register
  set_funcs[dest_type](result);
}
