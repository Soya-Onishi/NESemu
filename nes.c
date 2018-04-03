#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include"cpu_circuit.h"

#define MAX_CYCLE 1000

void one_cycle_in_cpu();
void graphic_monitor();

void nes_processing() {
  while(1) {
    one_cycle_in_cpu();
  }
}

void one_cycle_in_cpu() {
  static int remain_cycle = MAX_CYCLE;
  int cycle, over_cycle = 0;
  int ppu_cycle;

  cycle = fetch_instruction();
  
  remain_cycle -= cycle;
  if(remain_cycle < 0) {
    for(ppu_cycle = (cycle + remain_cycle) * 3; ppu_cycle > 0; ppu_cycle--) {
      //ppu process
    }
    cycle = -remain_cycle;

    remain_cycle += MAX_CYCLE;
    graphic_monitor();
  }

  for(ppu_cycle = cycle * 3; ppu_cycle > 0; ppu_cycle--) {
    //ppu_process
  }
}

void graphic_monitor() {
  int time;
  static int stdtime = 0;

  while(1) {
    time = glutGet(GLUT_ELAPSED_TIME);
    if(time - stdtime > (1000 / 60)) {
      stdtime = time;

      //draw dots

      return;
    }
  }
}