#ifndef HEADER_CPU
#define HEADER_CPU

#define DEBUG

extern int ready_for_drawing;
extern unsigned char nes_flag6;

void cpu();
void init_cpu();

#endif