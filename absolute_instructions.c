#include<stdio.h>
#include<stdlib.h>
#include"cpu_circuit.h"
#include"memory.h"

void set_absolute_inst();

void load_addr_lower_from_mem_by_pc();
void load_addr_upper_from_mem_by_pc();


void absolute_inst() {
  switch(now_inst.inst) {
    case ADC:
    case SBC:
    case AND:
    case ORA:
    case EOR:
    case ASL:
    case LSR:
    case ROR:
    case ROL:
    case INC:
    case DEC:

    case CMP:
    case CPX:
    case CPY:
    case LDA:
    case LDX:
    case LDY:
    case STA:
    case STX:
    case STY:
    case BIT:

    case JMP:

    case JSR:

    default:
      printf("unknown absolute instruction\n");
      exit(1);
  }
}

void set_absolute_inst() {
  circuit_functions[0] = load_addr_lower_from_mem_by_pc;
  circuit_functions[1] = load_addr_upper_from_mem_by_pc;
  circuit_functions[2] = ;
  circuit_functions[3] = ;
}

void set_absolute_inst_mem() {

}

void set_absolute_inst_jmp() {

}

void set_absolute_inst_jsr() {

}

void load_addr_lower_from_mem_by_pc() {
  registers.pc++;
  c_line.addr_lower = memory[registers.pc];
}

void load_addr_upper_from_mem_by_pc() {
  registers.pc++;
  c_line.addr_upper = memory[registers.pc];
}

void read_or_write_data_absolute_mem() {
  unsigned short offset = 0;

  offset |= c_line.addr_lower;
  offset |= (unsigned short)c_line.addr_upper << 8;

  switch(now_inst.inst) {
    case CMP:
    case CPX:
    case CPY:
      c_line.data = memory[offset];
      break;
    case LDA:
      registers.accumulator = memory[offset];
      break;
    case LDX:
    case LDY:
    case STA:
    case STX:
    case STY:
    case BIT:

    default:
      printf("error\n");
      exit(1);
  }
}