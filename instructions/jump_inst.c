#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"
#include"../ppu.h"

int jmp_absolute() {
  unsigned short addr;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();

  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;
  ppu_cycle();

  registers.pc = addr - 3; // 3 is byte length of jmp absolute instruction

  return 0;
}

int jmp_indirect() {
  unsigned short addr;
  unsigned short new_pc;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();

  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;
  ppu_cycle();
  ppu_cycle();


  new_pc = memory_read(addr);
  ppu_cycle();

  new_pc |= (unsigned short)memory_read(addr + 1) << 8;
  ppu_cycle();

  registers.pc = new_pc - 3; // 3 is byte length of jmp indirect instruction

  return 0;
}

int jsr_absolute() {
  unsigned short addr;
  unsigned short next_inst_addr;

  addr = memory_read(registers.pc + 1);
  ppu_cycle();

  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;
  ppu_cycle();

  next_inst_addr = registers.pc + 2;
  ppu_cycle();

  memory_write(0x0100 | registers.stack--, (unsigned char)(next_inst_addr >> 8));
  ppu_cycle();

  memory_write(0x0100 | registers.stack--, (unsigned char)next_inst_addr);
  ppu_cycle();

  registers.pc = addr - 3;  // 3 is byte length of jsr absolute instruction

  return 0;
}

int rts_implied() {
  unsigned short addr;

  ppu_cycle();
  ppu_cycle();

  addr = memory_read(0x0100 | ++registers.stack);
  ppu_cycle();

  addr |= (unsigned short)memory_read(0x0100 | ++registers.stack) << 8;
  ppu_cycle();

  ppu_cycle();

  registers.pc = addr; // 1 is byte length of rts implied instruction

  return 0;
}