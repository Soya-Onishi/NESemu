#include<stdio.h>
#include<stdlib.h>
#include"GL/glut.h"
#include"cpu_circuit.h"
#include"ppu.h"
#include"cpu.h"

int ready_for_drawing = 0;

unsigned char nes_flag6;

void cpu() {
  if(ready_for_drawing) {
    //display rendering result by openGL
    static int stdtime = 1000 / 60;
    int time;

    time = glutGet(GLUT_ELAPSED_TIME);

    if(time >= stdtime) {
      stdtime += 1000 / 60;
      glutPostRedisplay();
      ready_for_drawing = 0;
    }
  } else {
    fetch_instruction();
  }
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