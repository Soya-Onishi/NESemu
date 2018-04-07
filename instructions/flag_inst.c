#include"../cpu_circuit.h"
#include"../ppu.h"

int clc_implied() {
  registers.status &= ~STATUS_C;
  ppu_cycle();
  return 0;
}

int sec_implied() {
  registers.status |= STATUS_C;
  ppu_cycle();
  return 0;
}

int cli_implied() {
  registers.status &= ~STATUS_I;
  ppu_cycle();
  return 0;
}

int sei_implied() {
  registers.status |= STATUS_I;
  ppu_cycle();
  return 0;
}

int cld_implied() {
  registers.status &= ~STATUS_D;
  ppu_cycle();
  return 0;
}

int sed_implied() {
  registers.status |= STATUS_D;
  ppu_cycle();
  return 0;
}

int clv_implied() {
  registers.status &= ~STATUS_V;
  ppu_cycle();
  return 0;
}

