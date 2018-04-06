#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"

int exec_bcc();
int exec_bcs();
int exec_beq();
int exec_bne();
int exec_bvc();
int exec_bvs();
int exec_bpl();
int exec_bmi();

int branch_relative(int (*exec_branch)());

int exec_bcc() {
  return ~registers.status & STATUS_C;
}

int exec_bcs() {
  return registers.status & STATUS_C;
}

int exec_beq() {
  return registers.status & STATUS_Z;
}

int exec_bne() {
  return ~registers.status & STATUS_Z;
}

int exec_bvc() {
  return ~registers.status & STATUS_V;
}

int exec_bvs() {
  return registers.status & STATUS_V;
}

int exec_bpl() {
  return ~registers.status & STATUS_N;
}

int exec_bmi() {
  return registers.status & STATUS_N;
}

int branch_relative(int (*exec_branch)()) {
  char offset;
  unsigned short before, after;
  
  int additional_cycle = 0;

  offset = (char)memory_read(registers.pc + 1);
  before = registers.pc + 2;
  
  if(exec_branch()) {
    additional_cycle++;
    after += before + offset;

    if((after & 0xf0) != (before & 0xf0)) {
      additional_cycle++;
    }

    registers.pc = after - 2;
  }

  return additional_cycle;
}

int bcc_relative() {
  return branch_relative(exec_bcc);
}

int bcs_relative() {
  return branch_relative(exec_bcs);
}

int beq_relative() {
  return branch_relative(exec_beq);
}

int bne_relative() {
  return branch_relative(exec_bne);
}

int bvc_relative() {
  return branch_relative(exec_bvc);
}

int bvs_relative() {
  return branch_relative(exec_bvs);
}

int bpl_relative() {
  return branch_relative(exec_bpl);
}

int bmi_relative() {
  return branch_relative(exec_bmi);
}