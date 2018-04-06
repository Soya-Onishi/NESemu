#include "ppu.h"

#define MAX_SCANLINE 262
#define MAX_DOTS 341

static int dots = 0;
static int scanline = -1;
static int sprite_count = 0;
static int frame = 0;

void ppu_rendering() {
  
  if(scanline == 0 && dots == 0 && frame % 2 == 1) {
    dots++;
  }

  //process


  dots++;
  if(dots == MAX_DOTS) {
    dots = 0;
    scanline = (scanline + 1) % (MAX_SCANLINE);
  }
}