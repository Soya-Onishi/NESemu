#ifndef HEADER_INSTRUCTION
#define HEADER_INSTRUCTION

typedef enum {
  ADC,
  SBC,
  AND,
  ORA,
  EOR,
  ASL,
  LSR,
  ROL,
  ROR,
  BCC,
  BCS,
  BEQ,
  BNE,
  BVC,
  BVS,
  BPL,
  BMI,
  BIT,
  JMP,
  JSR,
  RTS,
  BRK,
  RTI,
  CMP,
  CPX,
  CPY,
  INC,
  DEC,
  INX,
  DEX,
  INY,
  DEY,
  CLC,
  SEC,
  CLI,
  SEI,
  CLD,
  SED,
  CLV,
  LDA,
  LDX,
  LDY,
  STA,
  STX,
  STY,
  TAX,
  TXA,
  TAY,
  TYA,
  TSX,
  TXS,
  PHA,
  PLA,
  PHP,
  PLP,
  NOP,
  INST_UNDEF
}instruction_name;

typedef enum {
  ADDR_ACCUMULATOR,
  ADDR_IMMEDIATE,
  ADDR_ABSOLUTE,
  ADDR_ZEROPAGE,
  ADDR_INDEX_ZEROPAGE_X,
  ADDR_INDEX_ZEROPAGE_Y,
  ADDR_INDEX_ABSOLUTE_X,
  ADDR_INDEX_ABSOLUTE_Y,
  ADDR_IMPLIED,
  ADDR_RELATIVE,
  ADDR_INDEX_INDIRECT,
  ADDR_INDIRECT_INDEX,
  ADDR_ABSOLUTE_INDIRECT,
  ADDR_UNDEF
}addressing;

typedef struct {
  int (*instruction)();
  char cycle;
  char length;
}instruction;

extern instruction instruction_set[];

#endif