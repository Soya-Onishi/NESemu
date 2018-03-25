#include"cpu_circuit.h"
#include"memory.h"

void jmp_inst_absolute_former() {
  c_line.addr_lower = memory[registers.pc];
}

void jmp_inst_absolute_latter() {
  c_line.addr_upper = memory[registers.pc];
  registers.pc = c_line.addr_lower;
  registers.pc |= (unsigned short)c_line.addr_upper << 8;
}

void jmp_inst_indirect_get_lower_addr() {
  registers.pc++;
  c_line.addr_lower = memory[registers.pc];
}

void jmp_inst_indirect_get_upper_addr() {
  registers.pc++;
  c_line.addr_upper = memory[registers.pc];
}

void jmp_inst_indirect_calc_next_addr() {
  c_line.data = c_line.addr_lower + 1;
}

void jmp_inst_indirect_get_pcl() {
  unsigned short offset;

  offset = c_line.addr_lower;
  offset = (unsigned short)c_line.addr_upper << 8;
  registers.pc = memory[offset];
}

void jmp_inst_indirect_get_pch() {
  unsigned short offset;

  offset = c_line.data;
  offset = (unsigned short)c_line.addr_upper << 8;
  registers.pc |= (unsigned short)memory[offset] << 8;
}

void jsr_inst_get_pcl() {
  c_line.data = memory[c_line.inst_head_pc + 1];
}

void jsr_inst_calc_push_addr() {
  unsigned short push_pc;

  push_pc = c_line.inst_head_pc + 2;
  c_line.addr_lower = (unsigned char)push_pc;
  c_line.addr_upper = (unsigned char)(push_pc >> 8);
}

void jsr_inst_push_old_pch() {
  unsigned short stack_pointer;

  stack_pointer = registers.stack;
  stack_pointer |= STACK_UPPER << 8;
  memory[stack_pointer] = c_line.addr_upper;
  
  registers.stack--;
}

void jsr_inst_push_old_pcl() {
  unsigned short stack_pointer;

  stack_pointer = registers.stack;
  stack_pointer |= STACK_UPPER << 8;
  memory[stack_pointer] = c_line.addr_lower;

  registers.stack--;
}

void jsr_inst_get_pch() {
  unsigned char upper;
  unsigned short new_pc;

  upper = memory[c_line.inst_head_pc + 2];
  new_pc = c_line.data;
  new_pc |= (unsigned short)upper << 8;
  registers.pc = new_pc;
}

void rts_inst_first_calc() {

}

void rts_inst_second_calc() {

}

void rts_inst_get_pcl() {
  unsigned short stack_pointer;
  
  registers.stack++;
  stack_pointer = registers.stack;
  stack_pointer |= STACK_UPPER << 8;

  c_line.addr_lower = memory[stack_pointer];
}

void rts_inst_get_pch() {
  unsigned short stack_pointer;
  
  registers.stack++;
  stack_pointer = registers.stack;
  stack_pointer |= STACK_UPPER << 8;

  c_line.addr_upper = memory[stack_pointer];
}

void rts_inst_calc_new_pc() {
  unsigned short new_pc;

  new_pc = c_line.addr_lower;
  new_pc = (unsigned short)c_line.addr_upper << 8;

  registers.pc = new_pc + 1;
}