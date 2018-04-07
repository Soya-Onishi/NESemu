#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"
#include"../ppu.h"

void exec_bit(unsigned char data);

void exec_bit(unsigned char data) {
  unsigned char result;

  result = registers.accumulator &  data;
  
  set_z_flag(result);
  set_n_flag(data);
  
  if(data & STATUS_V) {
    registers.status |= STATUS_V;
  } else {
    registers.status &= ~STATUS_V;
  }
}

int bit_zeropage() {
  unsigned char addr, data;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();
  addr = memory_read(addr);
  ppu_cycle();

  exec_bit(data);

  return 0;
}

int bit_absolute() {
  unsigned short addr;
  unsigned char data;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();
  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;
  ppu_cycle();

  data = memory_read(addr);
  ppu_cycle();
  
  exec_bit(data);

  return 0;
} 