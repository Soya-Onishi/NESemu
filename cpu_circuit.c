#include<stdio.h>
#include<stdlib.h>
#include"memory.h"
#include"instruction.h"
#include"accumulator_instructions.h"
#include"cpu_circuit.h"

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
unsigned char local_cycle;

unsigned int cpu_cycle;
instruction now_inst;
void (*circuit_functions[7])();

void fetch_instruction() {
  unsigned char op;
  instruction inst;

  op = memory[registers.pc];
  now_inst = instruction_set[op];
  local_cycle = 0;

  switch(now_inst.addr) {
    case ADDR_ACCUMULATOR:
      accumulator_inst();
    case ADDR_IMMEDIATE:
    case ADDR_ABSOLUTE:
    case ADDR_ZEROPAGE:
    case ADDR_INDEX_ZEROPAGE_X:
    case ADDR_INDEX_ZEROPAGE_Y:
    case ADDR_INDEX_ABSOLUTE_X:
    case ADDR_INDEX_ABSOLUTE_Y:
    case ADDR_IMPLIED:
    case ADDR_RELATIVE:
    case ADDR_INDEX_INDIRECT:
    case ADDR_INDIRECT_INDEX:
    case ADDR_ABSOLUTE_INDIRECT:
    case ADDR_UNDEF:
      printf("unknown addressing\n");
      exit(1);
  }
}



