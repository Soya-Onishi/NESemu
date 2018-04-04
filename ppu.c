#include <stdio.h>
#include <stdlib.h>
#include "ppu.h"


#define MAX_SCANLINE 262

ppu_registers ppu_reg;
char is_oddframe = 0;
char sprite_latch[8];
char sprite_x_counter[8];
char ppu_addr_bus;

char oam[256];
char second_oam[32];

void ppu_cycle() {
  static float cycles = 0;
  static float now_cycle = 0;

  cycles += 3.2;
  while(now_cycle < cycles) {
    

    now_cycle++;
  }
}