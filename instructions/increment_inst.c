#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"
#include"../ppu.h"

unsigned char exec_increment(unsigned char data);
unsigned char exec_decrement(unsigned char data);

int inc_dec_zeropage(unsigned char(*exec_inc_dec)(unsigned char));
int inc_dec_zeropage_x(unsigned char(*exec_inc_dec)(unsigned char));
int inc_dec_absolute(unsigned char(*exec_inc_dec)(unsigned char));
int inc_dec_absolute_x(unsigned char(*exec_inc_dec)(unsigned char));


unsigned char exec_increment(unsigned char data) {
  return data + 1;
}

unsigned char exec_decrement(unsigned char data) {
  return data - 1;
}

int inc_dec_zeropage(unsigned char(*exec_inc_dec)(unsigned char)) {
  unsigned char addr;
  unsigned char data;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();

  data = memory_read(addr);
  ppu_cycle();

  data = exec_inc_dec(data);
  ppu_cycle();

  memory_write(addr, data);
  ppu_cycle();

  return 0;
}

int inc_zeropage() {
  return inc_dec_zeropage(exec_increment);
}

int dec_zeropage() {
  return inc_dec_zeropage(exec_decrement);
}

int inc_dec_zeropage_x(unsigned char(*exec_inc_dec)(unsigned char)) {
  unsigned char addr;
  unsigned char data;

  addr = memory_read(registers.pc + 1); 
  ppu_cycle();

  addr += registers.index_x;
  ppu_cycle();

  data = memory_read(addr);
  ppu_cycle();

  data = exec_inc_dec(data);
  ppu_cycle();

  memory_write(addr, data);
  ppu_cycle();

  return 0;
}

int inc_zeropage_x() {
  return inc_dec_zeropage_x(exec_increment);
}

int dec_zeropage_x() {
  return inc_dec_zeropage_x(exec_decrement);
}

int inc_dec_absolute(unsigned char(*exec_inc_dec)(unsigned char)) {
  unsigned short addr;
  unsigned char data;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();

  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;
  ppu_cycle();

  data = memory_read(addr);
  ppu_cycle();

  data = exec_inc_dec(data);
  ppu_cycle();

  memory_write(addr, data);
  ppu_cycle();

  return 0;
}

int inc_absolute() {
  return inc_dec_absolute(exec_increment);
}

int dec_absolute() {
  return inc_dec_absolute(exec_decrement);
}

int inc_dec_absolute_x(unsigned char(*exec_inc_dec)(unsigned char)) {
  unsigned short addr, before;
  int additional_cycle = 0;
  unsigned char data;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();

  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;
  ppu_cycle();

  before = addr;
  addr += registers.index_x;
  if((addr & 0xf0) != (before & 0xf0)) {
    //additional_cycle++;
    ppu_cycle();
  }

  data = memory_read(addr);
  ppu_cycle();

  data = exec_inc_dec(data);
  ppu_cycle();

  memory_write(addr, data);
  ppu_cycle();

  return additional_cycle;
}

int inc_absolute_x() {
  return inc_dec_absolute_x(exec_increment);
}

int dec_absolute_x() {
  return inc_dec_absolute_x(exec_decrement);
}

int inx_implied() {
  registers.index_x++;
  ppu_cycle();
  return 0;
}

int dex_implied() {
  registers.index_x--;
  ppu_cycle();
  return 0;
}

int iny_implied() {
  registers.index_y++;
  ppu_cycle();
  return 0;
}

int dey_implied() {
  registers.index_y--;
  ppu_cycle();
  return 0;
}