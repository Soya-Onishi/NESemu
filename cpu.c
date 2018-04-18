#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include"GL/glut.h"
#include"cpu_circuit.h"
#include"ppu.h"
#include"cpu.h"
#include "memory.h"

#define FPS_TIME (1000000 / 60)

void test_log_load();
void set_timer();
unsigned long get_time();

int ready_for_drawing = 0;

unsigned char nes_flag6;

struct timeval start, now;

void cpu() {
  if(ready_for_drawing) {
    //display rendering result by openGL
    static int count = 0;
    static int counter = 0;

    if(get_time() >= FPS_TIME) {
      glutForceJoystickFunc();
      set_timer();
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
  intr_flags.reset = 1;

  registers.accumulator = 0;
  registers.index_x = 0;
  registers.index_y = 0;
  registers.status = 0x34;
  registers.stack = 0;

  init_memory();
  set_timer();
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

void set_timer() {
  gettimeofday(&start, NULL);
}

unsigned long get_time() {
  gettimeofday(&now, NULL);

  return (now.tv_sec * 1000000 + now.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
}