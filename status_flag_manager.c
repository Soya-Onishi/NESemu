#include"cpu_circuit.h"

void set_z_flag(unsigned char num) {
  if(num == 0) {
    registers.status |= STATUS_Z;
  } else {
    registers.status &= ~STATUS_Z;
  }
}

void set_n_flag(unsigned char num) {
  if(num & (1 << 7)) {
    registers.status |= STATUS_N;
  } else {
    registers.status &= ~STATUS_N;
  }
}