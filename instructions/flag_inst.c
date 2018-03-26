#include"cpu_circuit.h"

int clc_implied() {
  registers.status &= ~STATUS_C;
  return 0;
}

int sec_implied() {
  registers.status |= STATUS_C;
  return 0;
}

int cli_implied() {
  registers.status &= ~STATUS_I;
  return 0;
}

int sei_implied() {
  registers.status |= STATUS_I;
  return 0;
}

int cld_implied() {
  registers.status &= ~STATUS_D;
  return 0;
}

int sed_implied() {
  registers.status |= STATUS_D;
  return 0;
}

int clv_implied() {
  registers.status &= ~STATUS_V;
  return 0;
}

