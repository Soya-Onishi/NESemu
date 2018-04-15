#include<stdio.h>
#include<stdlib.h>
#include"GL/glut.h"
#include"cpu_circuit.h"
#include"ppu.h"
#include"cpu.h"
#include "memory.h"

void test_log_load();

int ready_for_drawing = 0;

unsigned char nes_flag6;

void cpu() {
  if(ready_for_drawing) {
    //display rendering result by openGL
    static int stdtime = 1000 / 60;
    static int count = 0;
    int time;

    time = glutGet(GLUT_ELAPSED_TIME);

    if(time >= stdtime) {
      stdtime = time + 1000 / 60;
      glutPostRedisplay();
      ready_for_drawing = 0;
    } else {
      printf("faster\n");
    }
  } else {
    fetch_instruction();
  }
}

void init_cpu() {
  intr_flags.nmi = 0;
  intr_flags.irq = 0;
  intr_flags.reset = 1;

  registers.accumulator = 0;
  registers.index_x = 0;
  registers.index_y = 0;
  registers.status = 0x34;
  registers.stack = 0;

  init_memory();

  //test_log_load();
}

void test_log_load() {
  int max = 1024;
  int i;
  FILE *fp;
  char str[512];

  fp = fopen("nestest.log", "r");
  tester = (test_status*)malloc(sizeof(test_status) * max);
  
  for(i = 0; i < 3; i++) {
    fgets(str, 512, fp);
    printf("%s", str);
    //sscanf(str, "%x  %x %x %x  A:%x X:%x Y:%x P:%x SP:%x CYC:%x",
    //&(tester[i].pc), &(tester[i].content[0]), &(tester[i].content[1]), &(tester[i].content[2]), 
    //&(tester[i].a), &(tester[i].x), &(tester[i].y), &(tester[i].p), &(tester[i].sp), &(tester[i].ppu_cycle));

    printf("%4X  %2X %2X %2X  A:%2X X:%2X Y:%2X P:%2X SP:%2X CYC:%3X\n\n",
    tester[i].pc, tester[i].content[0], tester[i].content[1], tester[i].content[2], 
    tester[i].a, tester[i].x, tester[i].y, tester[i].p, tester[i].sp, tester[i].ppu_cycle);
  }  
}