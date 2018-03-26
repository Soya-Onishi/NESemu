#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"

int pha_implied() {
  memory[registers.stack--] = registers.accumulator;
  return 0;
}

int pla_implied() {
  registers.accumulator = memory[++registers.stack];
  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
  return 0;
}

int php_implied() {
  memory[registers.stack--] = registers.status | STATUS_B;
  return 0;
}

int plp_implied() {
  registers.status = memory[++registers.stack];
  return 0;
}