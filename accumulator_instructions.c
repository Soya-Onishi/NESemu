#include<stdio.h>
#include<stdlib.h>

#include "cpu_circuit.h"

void accumulator_calc();

void accumulator_inst() {
  circuit_functions[0] = accumulator_calc;
  circuit_functions[1] = fetch_instruction;
}

void accumulator_calc() {
  unsigned char tmp;

  registers.pc++;

  switch(now_inst.inst) {
    case ASL:
      registers.status &= ~(STATUS_C) | (registers.accumulator >> 7);
      registers.accumulator <<= 1;
      break;
    case LSR:
      registers.status &= ~(STATUS_C) | registers.accumulator;
      registers.accumulator >>= 1;
      registers.accumulator &= 0x7f; // 0x7f = 01111111
      break;
    case ROL:
      tmp = registers.accumulator >> 7;
      registers.accumulator <<= 1;
      registers.accumulator |= STATUS_C & registers.status;
      registers.status &= ~(STATUS_C) | tmp;
      break;
    case ROR:
      tmp = registers.accumulator;
      registers.accumulator >>= 1;

      //reset accumulator bit7 to 0 and get status register's C flag. at the end, set accumulator bit7 to C flag bit  
      registers.accumulator = (registers.accumulator & 0x7f) | ((registers.status & STATUS_C) << 7);

      registers.status &= ~(STATUS_C) | tmp;
      break;
    default:
      printf("unknown accumulator instruction\n");
      exit(1);
  }

  registers.status &= ~(STATUS_N) | registers.accumulator;
  
  if(registers.accumulator == 0) {
    registers.status |= STATUS_Z;
  }else{
    registers.status &= ~STATUS_Z;
  }
}