#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"
#include"../ppu.h"

int pha_implied() {
  ppu_cycle();
  memory_write(registers.stack--, registers.accumulator);
  ppu_cycle();
  return 0;
}

int pla_implied() {
  ppu_cycle();
  registers.accumulator = memory_read(++registers.stack);
  ppu_cycle();
  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
  ppu_cycle();
  return 0;
}

int php_implied() {
  ppu_cycle();
  memory_write(registers.stack--, registers.status | STATUS_B);
  ppu_cycle();
  return 0;
}

int plp_implied() {
  ppu_cycle();
  registers.status = memory_read(++registers.stack);
  ppu_cycle();
  ppu_cycle();
  return 0;
}