#include "ppu.h"
#include "bg_render.h"
#include "sprite_render.h"
#include "rendering.h"

#define MAX_SCANLINE 262
#define MAX_DOTS 341

static int dots = 0;
static int scanline = -1;
static int frame = 0;

void ppu_rendering() {
  
  if(scanline == 0 && dots == 0 && frame % 2 == 1) {
    dots++;
  }
  
  //process
  bg_render();
  sprite_render();
  rendering();

  dots++;
  if(dots == MAX_DOTS) {
    dots = 0;
    scanline = (scanline + 1) % (MAX_SCANLINE);
  }
}

int get_dots() {
  return dots;
}

int get_scanline() {
  return scanline;
}

int get_frame() {
  return frame;
}