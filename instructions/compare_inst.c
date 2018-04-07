#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"

void exec_compare(unsigned char a, unsigned char m);

void exec_cmp(unsigned char data);
void exec_cpx(unsigned char data);
void exec_cpy(unsigned char data);

void compare_immediate(void (*exec_comp)(unsigned char));
void compare_zeropage(void (*exec_comp)(unsigned char));
void compare_absolute(void (*exec_comp)(unsigned char));

int cmp_absolute_index(unsigned char index);

void exec_compare(unsigned char a, unsigned char m) {
  char result;

  result = a - m;
  if(result >= 0) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  set_z_flag(result);
  set_n_flag(result);
}

void exec_cmp(unsigned char data) {
  exec_compare(registers.accumulator, data);
}

void exec_cpx(unsigned char data) {
  exec_compare(registers.index_x, data);
} 

void exec_cpy(unsigned char data) {
  exec_compare(registers.index_y, data);
}

void compare_immediate(void (*exec_comp)(unsigned char)) {
  exec_comp(memory_read(registers.pc + 1));
}

int cmp_immediate() {
  compare_immediate(exec_cmp);
  return 0;
}

int cpx_immediate() {
  compare_immediate(exec_cpx);
  return 0;
}

int cpy_immediate() {
  compare_immediate(exec_cpy);
  return 0;
}

void compare_zeropage(void (*exec_comp)(unsigned char)) {
  unsigned char addr;

  addr = memory_read(registers.pc + 1);
  exec_comp(memory_read(addr));
}

int cmp_zeropage() {
  compare_zeropage(exec_cmp);
  return 0;
}

int cpx_zeropage() {
  compare_zeropage(exec_cpx);
  return 0;
}

int cpy_zeropage() {
  compare_zeropage(exec_cpy);
  return 0;
}

void compare_absolute(void (*exec_comp)(unsigned char)) {
  unsigned short addr;

  addr = memory_read(registers.pc + 1);
  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;

  exec_comp(memory_read(addr));
}

int cmp_absolute() {
  compare_absolute(exec_cmp);
  return 0;
}

int cpx_absolute() {
  compare_absolute(exec_cpx);
  return 0;
}

int cpy_absolute() {
  compare_absolute(exec_cpy);
  return 0;
}

int cmp_zeropage_x() {
  unsigned char addr;

  addr = memory_read(registers.pc + 1) + registers.index_x;
  exec_cmp(memory_read(addr));

  return 0;
}

int cmp_absolute_index(unsigned char index) {
  unsigned short addr, before;
  int additional_cycle = 0;

  addr = memory_read(registers.pc + 1);
  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;
  before = addr;

  addr += index;
  if((addr & 0xf0) != (before & 0xf0)) {
    additional_cycle++;
  }

  exec_cmp(memory_read(addr));

  return additional_cycle;
}

int cmp_absolute_x() {
  return cmp_absolute_index(registers.index_x);
}

int cmp_absolute_y() {
  return cmp_absolute_index(registers.index_y);
}

int cmp_indirect_x() {
  unsigned char addr;
  unsigned short data_addr;

  addr = memory_read(registers.pc + 1) + registers.index_x;

  data_addr |= (unsigned short)memory_read(addr) << 8;
  addr++;
  data_addr = memory_read(addr);
  
  exec_cmp(memory_read(data_addr));

  return 0;
}

int cmp_indirect_y() {
  unsigned char addr_addr;
  unsigned short addr, before;
  int additional_cycle = 0;

  addr_addr = memory_read(registers.pc + 1);
  addr = (unsigned short)memory_read(addr_addr) << 8;
  addr_addr++;
  addr |= (unsigned short)memory_read(addr_addr);

  before = addr;
  addr += registers.index_y;
  if((addr & 0xf0) != (before & 0xf0)) {
    additional_cycle++;
  }

  exec_cmp(memory_read(addr));

  return additional_cycle;
}