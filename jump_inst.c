#include"cpu_circuit.h"
#include"memory.h"

int jmp_absolute() {
  unsigned short addr;

  addr = memory[registers.pc + 1];
  addr |= (unsigned short)memory[registers.pc + 2] << 8;

  registers.pc = addr - 3; // 3 is byte length of jmp absolute instruction

  return 0;
}

int jmp_indirect() {
  unsigned short addr;

  addr = memory[registers.pc + 1];
  addr |= (unsigned short)memory[registers.pc + 2] << 8;
  registers.pc = memory[addr] - 3; // 3 is byte length of jmp indirect instruction

  return 0;
}

int jsr_absolute() {
  unsigned short addr;
  unsigned short next_inst_addr;

  addr = memory[registers.pc + 1];
  addr |= (unsigned short)memory[registers.pc + 2] << 8;

  next_inst_addr = registers.pc + 2;
  memory[registers.stack--] = (unsigned char)(next_inst_addr >> 8);
  memory[registers.stack--] = (unsigned char)next_inst_addr;

  registers.pc = addr - 3;  // 3 is byte length of jsr absolute instruction

  return 0;
}

int rts_implied() {
  unsigned short addr;

  addr = memory[++registers.stack];
  addr |= (unsigned short)memory[++registers.stack] << 8;
  addr++;
  
  registers.pc = addr - 1; // 1 is byte length of rts implied instruction

  return 0;
}