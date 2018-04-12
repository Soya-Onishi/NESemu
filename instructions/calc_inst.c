#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"
#include"../ppu.h"

void exec_adc(unsigned char data);
void exec_sbc(unsigned char data);
void exec_and(unsigned char data);
void exec_ora(unsigned char data);
void exec_eor(unsigned char data);

void calc_immediate(void (*calc_exec)(unsigned char));
void calc_zeropage(void (*exec_calc)(unsigned char));
void calc_zeropage_x(void (*exec_calc)(unsigned char));
void calc_absolute(void (*exec_calc)(unsigned char));
int calc_absolute_index(unsigned char index, void (*exec_calc)(unsigned char));
void calc_indirect_x(void (*exec_calc)(unsigned char));
int calc_indirect_y(void (*exec_calc)(unsigned char));

void exec_adc(unsigned char data) {
  unsigned char before = registers.accumulator;

  registers.accumulator += data + (registers.status & STATUS_C);

  if(before <= 0x7f && registers.accumulator >= 0x80) {
    registers.status |= STATUS_V;
  } else {
    registers.status &= ~STATUS_V;
  }

  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);

  if(registers.accumulator < before) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }
}

void exec_sbc(unsigned char data) {
  unsigned char before = registers.accumulator;

  registers.accumulator = registers.accumulator - data - (1 - (registers.status & STATUS_C));

  if(before >= 0x80 && registers.accumulator <= 0x7f) {
    registers.status |= STATUS_V;
  } else {
    registers.status &= ~STATUS_V;
  }

  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);

  if(registers.accumulator > before) {
    registers.status &= ~STATUS_C;
  } else {
    registers.status |= STATUS_C;
  }
}

void exec_and(unsigned char data) {
  registers.accumulator &= data;
  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
}

void exec_ora(unsigned char data) {
  registers.accumulator |= data;
  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
}

void exec_eor(unsigned char data) {
  registers.accumulator ^= data;
  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
}

void calc_immediate(void (*calc_exec)(unsigned char)) {
  unsigned char data;
  
  data = memory_read(registers.pc + 1);
  calc_exec(data);
}

int adc_immediate() {
  calc_immediate(exec_adc);
  return 0;
}

int sbc_immediate() {
  calc_immediate(exec_sbc);
  return 0;
}

int and_immediate() {
  calc_immediate(exec_and);
  return 0;
}

int ora_immediate() {
  calc_immediate(exec_ora);
  return 0;
}

int eor_immediate() {
  calc_immediate(exec_eor);
  return 0;
}

void calc_zeropage(void (*exec_calc)(unsigned char)) {
  unsigned char offset;
  unsigned char data;

  offset = memory_read(registers.pc + 1);
  ppu_cycle();

  data = memory_read(offset);
  ppu_cycle();

  exec_calc(data);
}

int adc_zeropage() {
  calc_zeropage(exec_adc);
  return 0;
}

int sbc_zeropage() {
  calc_zeropage(exec_sbc);
  return 0;
}

int and_zeropage() {
  calc_zeropage(exec_and);
  return 0;
}

int ora_zeropage() {
  calc_zeropage(exec_ora);
  return 0;
}

int eor_zeropage() {
  calc_zeropage(exec_eor);
  return 0;
}

void calc_zeropage_x(void (*exec_calc)(unsigned char)) {
  unsigned char offset;
  unsigned char data;

  offset = memory_read(registers.pc + 1);
  ppu_cycle();

  offset += registers.index_x;
  ppu_cycle();

  data = memory_read(offset);
  ppu_cycle();

  exec_calc(data);
}

int adc_zeropage_x() {
  calc_zeropage_x(exec_adc);
  return 0;
}

int sbc_zeropage_x() {
  calc_zeropage_x(exec_sbc);
  return 0;
}

int and_zeropage_x() {
  calc_zeropage_x(exec_and);
  return 0;
}

int ora_zeropage_x() {
  calc_zeropage_x(exec_ora);
  return 0;
}

int eor_zeropage_x() {
  calc_zeropage_x(exec_eor);
  return 0;
}

void calc_absolute(void (*exec_calc)(unsigned char)) {
  unsigned short offset;
  unsigned char data;

  offset = memory_read(registers.pc + 1);
  ppu_cycle();

  offset |= (unsigned short)memory_read(registers.pc + 2) << 8;
  ppu_cycle();

  data = memory_read(offset);
  ppu_cycle();

  exec_calc(data);
}

int adc_absolute() {
  calc_absolute(exec_adc);
  return 0;
}

int sbc_absolute() {
  calc_absolute(exec_sbc);
  return 0;
}

int and_absolute() {
  calc_absolute(exec_and);
  return 0;
}

int ora_absolute() {
  calc_absolute(exec_ora);
  return 0;
}

int eor_absolute() {
  calc_absolute(exec_eor);
  return 0;
}

int calc_absolute_index(unsigned char index, void (*exec_calc)(unsigned char)) {
  unsigned short offset;
  unsigned char offset_lower, offset_upper, before;
  unsigned char data;
  int additional_cycle = 0;

  before = offset_lower = memory_read(registers.pc + 1);
  ppu_cycle();
  
  offset_upper = memory_read(registers.pc + 2);
  ppu_cycle();

  offset_lower += index;
  if(offset_lower < before) {
    //additional_cycle = 1;
    ppu_cycle();
    offset_upper++;
  }

  offset = ((unsigned short)offset_upper << 8) + offset_lower;

  data = memory_read(offset);
  ppu_cycle();

  exec_calc(data);

  return additional_cycle;
}

int adc_absolute_x() {
  return calc_absolute_index(registers.index_x, exec_adc);
}

int adc_absolute_y() {
  return calc_absolute_index(registers.index_y, exec_adc);
}

int sbc_absolute_x() {
  return calc_absolute_index(registers.index_x, exec_sbc);
}

int sbc_absolute_y() {
  return calc_absolute_index(registers.index_y, exec_sbc);
}

int and_absolute_x() {
  return calc_absolute_index(registers.index_x, exec_and);
}

int and_absolute_y() {
  return calc_absolute_index(registers.index_y, exec_and);
}

int ora_absolute_x() {
  return calc_absolute_index(registers.index_x, exec_ora);
}

int ora_absolute_y() {
  return calc_absolute_index(registers.index_y, exec_ora);
}

int eor_absolute_x() {
  return calc_absolute_index(registers.index_x, exec_eor);
}

int eor_absolute_y() {
  return calc_absolute_index(registers.index_y, exec_eor);
}

void calc_indirect_x(void (*exec_calc)(unsigned char)) {
  unsigned char offset;
  unsigned short addr;
  unsigned char data;

  offset = memory_read(registers.pc + 1);
  ppu_cycle();

  offset += registers.index_x;
  ppu_cycle();

  addr = memory_read(offset);
  ppu_cycle();

  offset++;
  addr |= (unsigned short)memory_read(offset) << 8;
  ppu_cycle();

  data = memory_read(addr);
  ppu_cycle();

  exec_calc(data);
}

int adc_indirect_x() {
  calc_indirect_x(exec_adc);
  return 0;
}

int sbc_indirect_x() {
  calc_indirect_x(exec_sbc);
  return 0;
}

int and_indirect_x() {
  calc_indirect_x(exec_and);
  return 0;
}

int ora_indirect_x() {
  calc_indirect_x(exec_ora);
  return 0;
}

int eor_indirect_x() {
  calc_indirect_x(exec_eor);
  return 0;
}

int calc_indirect_y(void (*exec_calc)(unsigned char)) {
  unsigned short exec_addr, before;
  unsigned char addr;
  unsigned char data;
  int additional_cycle = 0;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();

  exec_addr = memory_read(addr);
  ppu_cycle();

  addr++;
  exec_addr |= memory_read(addr) << 8;
  ppu_cycle();

  before = exec_addr;
  exec_addr += registers.index_y;

  if((before & 0xFF00) != (exec_addr & 0xFF00)) {
    ppu_cycle();
  }

  data = memory_read(exec_addr);
  ppu_cycle();
  
  exec_calc(data);

  return additional_cycle;
}

int adc_indirect_y() {
  return calc_indirect_y(exec_adc);
}

int sbc_indirect_y() {
  return calc_indirect_y(exec_sbc);
}

int and_indirect_y() {
  return calc_indirect_y(exec_and);
}

int ora_indirect_y() {
  return calc_indirect_y(exec_ora);
}

int eor_indirect_y() {
  return calc_indirect_y(exec_eor);
}