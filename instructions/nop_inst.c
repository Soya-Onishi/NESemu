#include<stdio.h>
#include<stdlib.h>

int nop_implied() {
  return 0;
}

int unknown_opcode() {
  printf("unknown opcode\n");
  exit(1);
}