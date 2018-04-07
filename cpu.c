#include<stdio.h>
#include<stdlib.h>
#include"cpu_circuit.h"
#include"ppu.h"
#include"cpu.h"

void cpu() {
  fetch_instruction();
}

void init_cpu() {
  intr_flags.nmi = 0;
  intr_flags.irq = 0;
  intr_flags.reset = 0;

  registers.accumulator = 0;
  registers.index_x = 0;
  registers.index_y = 0;
  registers.status = 0x34;
  registers.stack = 0xFD;
}