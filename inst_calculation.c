#include<stdio.h>
#include<stdlib.h>
#include"cpu_circuit.h"
#include"memory.h"
#include"status_flag_manager.h"

void adc_inst(unsigned char num) {
  unsigned char tmp;

  tmp = registers.accumulator;
  registers.accumulator += num + (registers.status & STATUS_C);

  if(tmp <= 0x7f && registers.accumulator >= 0x80) {
    registers.status |= STATUS_V;
  } else {
    registers.status &= ~STATUS_V;
  }

  if(tmp > registers.accumulator) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  set_n_flag(registers.accumulator);
  set_z_flag(registers.accumulator);
}

void sbc_inst(unsigned char num) {
  unsigned char tmp;

  tmp = registers.accumulator;
  registers.accumulator -= (num + (1 - (registers.status & STATUS_C)));

  if(tmp >= 0x80 && registers.accumulator <= 0x7f) {
    registers.status |= STATUS_V;
  } else {
    registers.status &= ~STATUS_V;
  }

  if(tmp < registers.accumulator) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  set_n_flag(registers.accumulator);
  set_z_flag(registers.accumulator);
}

void and_inst(unsigned char num) {
  registers.accumulator &= num;

  set_n_flag(registers.accumulator);
  set_z_flag(registers.accumulator);
}

void ora_inst(unsigned char num) {
  registers.accumulator |= num;

  set_n_flag(registers.accumulator);
  set_z_flag(registers.accumulator);
}

void eor_inst(unsigned char num) {
  registers.accumulator ^= num;

  set_n_flag(registers.accumulator);
  set_z_flag(registers.accumulator);
}

unsigned char asl_inst(unsigned char num) {
  if(num & (1 << 7)) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }
  
  num <<= 1;
  
  set_z_flag(num);
  set_n_flag(num);

  return num;
}

unsigned char lsr_inst(unsigned char num) {
  if(num & (1 << 0)) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  num >>= 1;
  num &= 0x7f;

  set_z_flag(num);
  set_n_flag(num);

  return num;
}

unsigned char rol_inst(unsigned char num) {
  unsigned char old_status = registers.status;

  if(num & (1 << 7)) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  num <<= 1;
  
  if(old_status & STATUS_C) {
    num |= 0x1;
  } else {
    num &= 0xfe;
  }

  set_z_flag(num);
  set_n_flag(num);

  return num;
}

unsigned char ror_inst(unsigned char num) {
  unsigned char old_status = registers.status;
  
  if(num & (1 << 0)) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  num >>= 1;
  
  if(old_status & STATUS_C) {
    num |= 0x80;
  } else {
    num &= 0x7f;
  }

  set_n_flag(num);
  set_z_flag(num);

  return num;
}

void bit_inst(unsigned char num) {
  unsigned char res;

  res = num & registers.accumulator;

  set_z_flag(res);
  set_n_flag(num);
  registers.status = (registers.status & ~(STATUS_V)) | (num & STATUS_V);
}

