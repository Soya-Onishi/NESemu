#include<stdio.h>
#include<stdlib.h>
#include"../ppu.h"

int nop_implied() {
  ppu_cycle();
  return 0;
}

int unknown_opcode() {
  printf("unknown opcode\n");
  exit(1);
}