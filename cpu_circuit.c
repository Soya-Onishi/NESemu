#include<stdio.h>
#include<stdlib.h>
#include"memory.h"
#include"instruction.h"
#include"cpu_circuit.h"
#include"instructions/interrupt_inst.h"
#include"ppu.h"
#include"ppu_rendering.h"
#include"cpu.h"

void print_instruction(instruction inst);
void print_ppu_status(unsigned char opcode);
void print_cycle();

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

  #ifdef DEBUG
  {
    static int first = 1;
    if(first) {
      registers.pc = 0xC000;
      registers.status = 0x24;
      first = 0;
      is_reset = 0;
    }
  }
  #endif

  opcode = memory_read(registers.pc);
  ppu_cycle();
  
  inst = instruction_set[opcode];
  print_instruction(inst);
  additional_cycle = inst.instruction();

  //print_ppu_status(opcode);

  registers.pc += inst.length;

  return additional_cycle + inst.cycle;
}

void print_ppu_status(unsigned char opcode) {
  printf("%2X: %d %d\n", opcode, get_true_scanline(), get_true_dots());
}

void print_instruction(instruction inst) {
  #ifdef DEBUG
  int i;

  printf("%04X  ", registers.pc);
  for(i = 0; i < inst.length; i++) {
    printf("%02X ", memory[registers.pc + i]);
  }
  for(; i < 3; i++) {
    printf("   ");
  }

  printf(" A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%3d\n",
         registers.accumulator, registers.index_x, registers.index_y,
         registers.status, registers.stack, (ppu_cycle_number + 341 - 3) % 341);
  #endif
}

void print_cycle() {
  printf("CYC:%3d\n", ppu_cycle_number);
}


