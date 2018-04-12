#include<stdio.h>
#include<stdlib.h>
#include"memory.h"
#include"instruction.h"
#include"cpu_circuit.h"
#include"instructions/interrupt_inst.h"
#include"ppu.h"

void print_instruction(instruction inst);

int test_start = 0;
test_status *tester;

typedef enum {
  INTERRUPT_RESET_UPPER = 0xfffd,
  INTERRUPT_RESET_LOWER = 0xfffc,
  INTERRUPT_NMI_UPPER = 0xfffb,
  INTERRUPT_NMI_LOWER = 0xfffa,
  INTERRUPT_IRQ_UPPER = 0xffff,
  INTERRUPT_IRQ_LOWER = 0xfffe,
  INTERRUPT_BRK_UPPER = 0xffff,
  INTERRUPT_BRK_LOWER = 0xfffe
}interrupt;

#define STACK_UPPER 0x01

cpu_register registers;
circuit_line c_line;
interrupt_flags intr_flags;
unsigned char local_cycle;

unsigned int cpu_cycle;
instruction now_inst;
void (*circuit_functions[7])();

int fetch_instruction() {
  instruction inst;
  unsigned char opcode;
  int additional_cycle;

  if(intr_flags.reset) {
    ppu_cycle();
    reset_implied();
    return 6;
  }

  if(intr_flags.nmi) {
    ppu_cycle();
    nmi_implied();
    return 7;
  }

  if(intr_flags.irq) {
    ppu_cycle();
    irq_implied();
    return 7;
  }

  opcode = memory_read(registers.pc);
  ppu_cycle();
  
  inst = instruction_set[opcode];
  //print_instruction(inst);
  additional_cycle = inst.instruction();
  registers.pc += inst.length;

  return additional_cycle + inst.cycle;
}

void print_instruction(instruction inst) {
  int i;

  if(!test_start) return;

  printf("%04X  ", registers.pc);
  for(i = 0; i < inst.length; i++) {
    printf("%02X ", memory[registers.pc + i]);
  }
  printf("\n");
}


