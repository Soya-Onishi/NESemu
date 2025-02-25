#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"
#include"../ppu.h"

void exec_compare(unsigned char a, unsigned char m);

void exec_cmp(unsigned char data);
void exec_cpx(unsigned char data);
void exec_cpy(unsigned char data);

void compare_immediate(void (*exec_comp)(unsigned char));
void compare_zeropage(void (*exec_comp)(unsigned char));
void compare_absolute(void (*exec_comp)(unsigned char));

int cmp_absolute_index(unsigned char index);

void exec_compare(unsigned char a, unsigned char m) {
  short result;

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
  ppu_cycle();
  exec_compare(registers.accumulator, data);
}

void exec_cpx(unsigned char data) {
  ppu_cycle();
  exec_compare(registers.index_x, data);
} 

void exec_cpy(unsigned char data) {
  ppu_cycle();
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
  ppu_cycle();

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
  ppu_cycle();

  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;
  ppu_cycle();

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
  ppu_cycle();
  ppu_cycle();

  exec_cmp(memory_read(addr));

  return 0;
}

int cmp_absolute_index(unsigned char index) {
  unsigned short addr, before;
  int additional_cycle = 0;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();

  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;
  ppu_cycle();

  before = addr;

  addr += index;
  if((addr & 0xFF00) != (before & 0xFF00)) {
    //additional_cycle++;
    ppu_cycle();
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
  unsigned char addr, data;
  unsigned short exec_addr;

  addr = memory_read(registers.pc + 1) + registers.index_x;
  ppu_cycle();
  ppu_cycle();

  exec_addr = memory_read(addr);
  addr++;
  ppu_cycle();

  exec_addr |= memory_read(addr) << 8;
  ppu_cycle();
  
  data = memory_read(exec_addr);

  exec_cmp(data);

  return 0;
}

int cmp_indirect_y() {
  unsigned char addr, data;
  unsigned short exec_addr, before;
  int additional_cycle = 0;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();

  exec_addr = memory_read(addr);
  addr++;
  ppu_cycle();

  exec_addr |= (unsigned short)memory_read(addr) << 8;
  ppu_cycle();

  before = exec_addr;
  exec_addr += registers.index_y;
  if((exec_addr & 0xFF00) != (before & 0xFF00)) {
    //additional_cycle++;
    ppu_cycle();
  }

  data = memory_read(exec_addr);

  exec_cmp(data);

  return additional_cycle;
}