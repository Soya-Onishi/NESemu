#include <stdio.h>
#include <stdlib.h>
#include "ppu.h"
#include "ppu_rendering.h"


#define MAX_SCANLINE 262

void init_display_color();
unsigned short convert_address(unsigned short addr);
void init_nametable();

void vram_write(unsigned short addr, unsigned char data);
unsigned char vram_read(unsigned short addr);

ppu_registers ppu_reg;
ppu_rendering_reg ppu_render_info;
rendering_sprite sprite[8];

unsigned char ppu_addr_bus = 0;
unsigned char ppu_data_bus = 0;
unsigned char vram[0x4000];

unsigned char nt_latch;
unsigned char at_latch;
unsigned char bg_latch[2];

unsigned short bg_pattern_reg[2];
unsigned short bg_attr_reg[2];
unsigned char sprite_zero_exist;

unsigned char oam[64][4];
unsigned char second_oam[8][4];

unsigned short rendering_addrs[240][256];
int *rendering_color[240][256];

unsigned char pallet_color[0x40][3] = {
  {84, 84, 84}, {0, 30, 116}, {8, 16, 144}, {48, 0, 136}, {68, 0, 100}, {92, 0, 48}, {84, 4, 0}, {60, 24, 0}, 
  {32, 42, 0}, {8, 58, 0}, {0, 64, 0}, {0, 60, 0}, {0, 50, 60}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
  {152, 150, 152}, {8, 76, 196}, {48, 50, 236}, {92, 30, 228}, {136, 20, 176}, {160, 20, 100}, {152, 34, 32}, {120, 60, 0},
  {84, 90, 0}, {40, 114, 0}, {8, 124, 0}, {0, 118, 40}, {0, 102, 120}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
  {236, 238, 236}, {76, 154, 236}, {120, 124, 236}, {176, 98, 236}, {228, 84, 236}, {236, 88, 180}, {236, 106, 100}, {212, 136, 32},
  {160, 170, 0}, {116, 196, 0}, {76, 208, 32}, {56, 204, 108}, {56, 180, 204}, {60, 60, 60}, {0, 0, 0}, {0, 0, 0},
  {236, 238, 236}, {168, 204, 236}, {188, 188, 236}, {212, 178, 236}, {236, 174, 236}, {236, 174, 212}, {236, 180, 176}, {228, 196, 144}, 
  {204, 210, 120}, {180, 222, 120}, {168, 226, 144}, {152, 226, 180}, {160, 214, 228}, {160, 162, 160}, {0, 0, 0}, {0, 0, 0}
};

void ppu_cycle() {
  int i;

  for(i = 0; i < 3; i++) {
    ppu_rendering();
  }


  /*
  static float cycles = 0;
  static float now_cycle = 0;

  cycles += 3.2;
  while(now_cycle < cycles) {
    

    now_cycle += 1.0;
  }
  */
}

void init_ppu() {
  ppu_reg.ctrl = 0;
  ppu_reg.mask = 0;
  ppu_reg.status = 0;
  ppu_reg.oamaddr = 0;
  ppu_render_info.toggle = 0;
  ppu_reg.scroll = 0;
  ppu_reg.ppuaddr = 0;
  ppu_reg.oamdata = 0;

  init_display_color();
  init_nametable();
}

void reset_ppu() {
  ppu_reg.ctrl = 0;
  ppu_reg.mask = 0;
  ppu_reg.status &= 0x80;

  //ppu_reg.oamaddr is unchanged
  
  ppu_render_info.toggle = 0;
  ppu_reg.scroll = 0;
  ppu_reg.ppuaddr = 0;
  ppu_reg.oamdata = 0;

  init_display_color();
}

void init_display_color() {
  int x, y;

  for(y = 0; y < 240; y++) {
    for(x = 0; x < 256; x++) {
      rendering_color[y][x] = (int*)pallet_color[0x39];
    }
  }
}

void vram_write(unsigned short addr, unsigned char data) {
  addr = convert_address(addr);
  vram[addr] = data;
}

unsigned char vram_read(unsigned short addr) {
  addr = convert_address(addr);
  return vram[addr];
}

unsigned short convert_address(unsigned short addr) {
  if(addr >= 0x2000 && addr <= 0x3EFF) {
    addr &= 0x2FFF;

    if(/*vertical mirroring*/) {
      addr &= 0xF4FF;
    } else if(/*horizontal mirroring*/) {
      addr &= 0xF8FF;
    }
  } else if(addr >= 0x3F00 && addr <= 0x3FFF) {
    addr &= 0xFF1F;
  }

  return addr;
}

void init_nametable() {
  unsigned short addr;

  for(addr = 0x2000; addr < 0x2FFF; addr++) {
    vram_write(addr, 0xFF);
  }
}