#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"
#include"../ppu.h"

int pha_implied() {
  unsigned short addr = 0x0100 + registers.stack--;

  ppu_cycle();
  memory_write(addr, registers.accumulator);
  ppu_cycle();

  return 0;
}

int pla_implied() {
  unsigned short addr = 0x0100;
  registers.stack++;
  addr += registers.stack;

  ppu_cycle();
  registers.accumulator = memory_read(addr);
  ppu_cycle();
  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
  ppu_cycle();
  return 0;
}

int php_implied() {
  unsigned short addr = 0x0100 + registers.stack--;

  ppu_cycle();

  memory_write(addr, registers.status | STATUS_B);
  ppu_cycle();
  return 0;
}

int plp_implied() {
  unsigned short addr = 0x0100;
  registers.stack++;
  addr += registers.stack;

  ppu_cycle();
  registers.status = memory_read(addr);
  ppu_cycle();
  ppu_cycle();
  return 0;
}