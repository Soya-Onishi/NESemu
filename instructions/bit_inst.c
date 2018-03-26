#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"

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

  addr = memory[registers.pc + 1];
  data = memory[addr];
  exec_bit(data);

  return 0;
}

int bit_absolute() {
  unsigned short addr;
  unsigned char data;

  addr = memory[registers.pc + 1];
  addr |= (unsigned short)memory[registers.pc + 2]  << 8;

  data = memory[addr];
  exec_bit(data);

  return 0;
} 