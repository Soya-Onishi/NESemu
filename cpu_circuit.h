#ifndef HEADER_CPU_CIRCUIT
#define HEADER_CPU_CIRCUIT

#include "instruction.h"

int fetch_instruction();

typedef enum {
  STATUS_N = 1 << 7,
  STATUS_V = 1 << 6,
  STATUS_B = 1 << 4,
  STATUS_D = 1 << 2,
  STATUS_I = 1 << 3,
  STATUS_Z = 1 << 1,
  STATUS_C = 1 << 0
}status_flags;

typedef struct cpu_register{
  unsigned char accumulator;
  unsigned char index_x;
  unsigned char index_y;
  unsigned short pc;
  unsigned char stack;
  unsigned char status;
}cpu_register;

typedef struct circuit_line{
  unsigned char data;
  unsigned char addr_lower;
  unsigned char addr_upper;
}circuit_line;

typedef struct interrupt_flags {
  unsigned char nmi;
  unsigned char irq;
  unsigned char reset;
} interrupt_flags;

extern instruction now_inst;
extern void (*circuit_functions[])();
extern cpu_register registers;
extern circuit_line c_line;
extern interrupt_flags intr_flags;

#define STACK_UPPER 0x01

#endif