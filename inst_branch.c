#include<stdio.h>
#include<stdlib.h>
#include"cpu_circuit.h"
#include"memory.h"

unsigned char bcc_inst() {
  return (registers.status & STATUS_C) ^ STATUS_C;
}

unsigned char bcs_inst() {
  return registers.status & STATUS_C;
}

unsigned char beq_inst() {
  return registers.status & STATUS_Z;
}

unsigned char bne_inst() {
  return (registers.status & STATUS_Z) ^ STATUS_Z;
}

unsigned char bvc_inst() {
  return (registers.status & STATUS_V) ^ STATUS_V;
}

unsigned char bvs_inst() {
  return registers.status & STATUS_V;
}

unsigned char bpl_inst() {
  return (registers.status & STATUS_N) ^ STATUS_N;
}

unsigned char bmi_inst() {
  return registers.status & STATUS_N;
}