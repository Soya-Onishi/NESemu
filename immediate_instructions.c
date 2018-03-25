#include<stdio.h>
#include<stdlib.h>
#include"cpu_circuit.h"
#include"memory.h"
#include"status_flag_manager.c"

void get_data_from_memory_by_pc();
void calc_and_fetch_instruction();
void immediate_calc();

void immediate_adc();
void immediate_sbc();
void immediate_and();
void immediate_ora();
void immediate_eor();
void immediate_cmp();
void immediate_cpx();
void immediate_cpy();
void immediate_lda();
void immediate_ldx();
void immediate_ldy();

void immediate_inst() {
  circuit_functions[0] = get_data_from_memory_by_pc;
  circuit_functions[1] = calc_and_fetch_instruction;
}

void get_data_from_memory_by_pc() {
  registers.pc++;
  c_line.data = memory[registers.pc];
}

void calc_and_fetch_instruction() {
  registers.pc++;
  
  immediate_calc();
  fetch_instruction();
}

void immediate_calc() {

  switch(now_inst.inst) {
    case ADC:
      immediate_adc();
      break;
    case SBC:
      immediate_sbc();
      break;
    case AND:
      immediate_and();
      break;
    case ORA:
      immediate_ora();
      break;
    case EOR:
      immediate_eor();
      break;
    case CMP:
      immediate_cmp();
      break;
    case CPX:
      immediate_cpx();
      break;
    case CPY:
      immediate_cpy();
      break;
    case LDA:
      immediate_lda();
      break;
    case LDX:
      immediate_ldx();
      break;
    case LDY:
      immediate_ldy();
      break;
    default:
      printf("unknown immediate instruction\n");
      exit(1);
  }
}

void immediate_adc() {
  unsigned char tmp;

  tmp = registers.accumulator;
  registers.accumulator += c_line.data + (registers.status & STATUS_C);

  if(tmp <= 0x7f && registers.accumulator >= 0x80) {
    registers.status |= STATUS_V;
  } else {
    registers.status &= ~STATUS_V;
  }

  if(tmp > registers.accumulator) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  set_n_flag(registers.accumulator);
  set_z_flag(registers.accumulator);
}

void immediate_sbc() {
  unsigned char tmp;

  tmp = registers.accumulator;
  registers.accumulator -= (c_line.data + (1 - (registers.status & STATUS_C)));

  if(tmp >= 0x80 && registers.accumulator <= 0x7f) {
    registers.status |= STATUS_V;
  } else {
    registers.status &= ~STATUS_V;
  }

  if(tmp < registers.accumulator) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  set_n_flag(registers.accumulator);
  set_z_flag(registers.accumulator);
}

void immediate_and() {
  registers.accumulator &= c_line.data;
  
  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
}

void immediate_ora() {
  registers.accumulator |= c_line.data;

  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
}

void immediate_eor() {
  registers.accumulator ^= c_line.data;

  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
}

void immediate_cmp() {
  unsigned char tmp;

  if(registers.accumulator >= c_line.data) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  tmp = registers.accumulator - c_line.data;
  
  set_z_flag(tmp);
  set_n_flag(tmp);
}

void immediate_cpx() {
  unsigned char tmp;

  if(registers.index_x >= c_line.data) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  tmp = registers.index_x - c_line.data;

  set_z_flag(tmp);
  set_n_flag(tmp);
}

void immediate_cpy() {
  unsigned char tmp;

  if(registers.index_y >= c_line.data) {
    registers.status |= STATUS_C;
  } else {
    registers.status &= ~STATUS_C;
  }

  tmp = registers.index_y - c_line.data;

  set_z_flag(tmp);
  set_n_flag(tmp);
}

void immediate_lda() {
  registers.accumulator = c_line.data;

  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
}

void immediate_ldx() {
  registers.index_x = c_line.data;

  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
}

void immediate_ldy() {
  registers.index_y = c_line.data;

  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
}

