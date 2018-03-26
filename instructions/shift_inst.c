#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"

unsigned char exec_asl(unsigned char data);
unsigned char exec_lsr(unsigned char data);
unsigned char exec_rol(unsigned char data);
unsigned char exec_ror(unsigned char data);

void shift_accumulator(unsigned char (*exec_shift)(unsigned char));
void shift_zeropage(unsigned char (*exec_shift)(unsigned char));
void shift_zeropage_x(unsigned char (*exec_shift)(unsigned char));
void shift_absolute(unsigned char(*exec_shift)(unsigned char));
int shift_absolute_x(unsigned char(*exec_shift)(unsigned char));


unsigned char exec_asl(unsigned char data) {
  if(data & (1 << 7)) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  data <<= 1;

  set_z_flag(data);
  set_n_flag(data);

  return data;
}

unsigned char exec_lsr(unsigned char data) {
  if(data & (1 << 0)) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  data >>= 1;
  data &= 0x7f;

  set_n_flag(data);
  set_z_flag(data);

  return data;
}

unsigned char exec_rol(unsigned char data) {
  unsigned char old_status = registers.status;

  if(data & (1 << 7)) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  data <<= 1;
  
  if(old_status & STATUS_C) {
    data |= 0x1;
  } else {
    data &= ~0x1;
  }

  set_z_flag(data);
  set_n_flag(data);

  return data;
}

unsigned char exec_ror(unsigned char data) {
  unsigned char old_status = registers.status;

  if(data & (1 << 0)) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  data >>= 1;

  if(old_status & STATUS_C) {
    data |= (1 << 7);
  } else {
    data &= ~(1 << 7);
  }

  set_n_flag(data);
  set_z_flag(data);

  return data;
}



void shift_accumulator(unsigned char (*exec_shift)(unsigned char)) {
  registers.accumulator = exec_shift(registers.accumulator);
}

int asl_accumulator() {
  shift_accumulator(exec_asl);
  return 0;
}

int lsr_accumulator() {
  shift_accumulator(exec_lsr);
  return 0;
}

int rol_accumulator() {
  shift_accumulator(exec_rol);
  return 0;
}

int ror_accumulator() {
  shift_accumulator(exec_ror);
  return 0;
}

void shift_zeropage(unsigned char (*exec_shift)(unsigned char)) {
  unsigned char data;
  unsigned char addr;

  addr = memory[registers.pc + 1];
  data = memory[addr];
  memory[addr] = exec_shift(data);
}

int asl_zeropage() {
  shift_zeropage(exec_asl);
  return 0;
}

int lsr_zeropage() {
  shift_zeropage(exec_lsr);
  return 0;
}

int rol_zeropage() {
  shift_zeropage(exec_rol);
  return 0;
}

int ror_zeropage() {
  shift_zeropage(exec_ror);
  return 0;
}

void shift_zeropage_x(unsigned char (*exec_shift)(unsigned char)) {
  unsigned char addr;
  unsigned char data;

  addr = memory[registers.pc + 1];
  addr += registers.index_x;
  data = memory[addr];

  memory[addr] = exec_shift(data);
}

int asl_zeropage_x() {
  shift_zeropage_x(exec_asl);
  return 0;
}

int lsr_zeropage_x() {
  shift_zeropage_x(exec_lsr);
  return 0;
}

int rol_zeropage_x() {
  shift_zeropage_x(exec_rol);
  return 0;
}

int ror_zeropage_x() {
  shift_zeropage_x(exec_ror);
  return 0;
}

void shift_absolute(unsigned char(*exec_shift)(unsigned char)) {
  unsigned short addr;
  unsigned char data;

  addr = memory[registers.pc + 1];
  addr |= (unsigned short)memory[registers.pc + 2] << 8;
  data = memory[addr];

  memory[addr] = exec_shift(data);
}

int asl_absolute() {
  shift_absolute(exec_asl);
  return 0;
}

int lsr_absolute() {
  shift_absolute(exec_lsr);
  return 0;
}

int rol_absolute() {
  shift_absolute(exec_rol);
  return 0;
}

int ror_absolute() {
  shift_absolute(exec_ror);
  return 0;
}

int shift_absolute_x(unsigned char(*exec_shift)(unsigned char)) {
  unsigned char addr_lower, addr_upper, before;
  unsigned short addr;
  unsigned char data;
  int additional_cycle = 0;

  before = addr_lower = memory[registers.pc + 1];
  addr_upper = memory[registers.pc + 2];

  addr_lower += registers.index_x;
  if(addr_lower < before) {
    addr_upper++;
    additional_cycle++;
  }

  addr = ((unsigned short)addr_upper << 8) + addr_lower;
  data = memory[addr];
  memory[addr] = exec_shift(data);

  return additional_cycle;
}

int asl_absolute_x() {
  return shift_absolute_x(exec_asl);
}

int lsr_absolute_x() {
  return shift_absolute_x(exec_lsr);
}

int rol_absolute_x() {
  return shift_absolute_x(exec_rol);
}

int ror_absolute_x() {
  return shift_absolute_x(exec_ror);
}