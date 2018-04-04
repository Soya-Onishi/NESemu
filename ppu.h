#ifndef HEADER_PPU
#define HEADER_PPU

typedef struct ppu_registers {
  unsigned short v;
  unsigned short t;
  unsigned char toggle;
  unsigned char fine_x;
  unsigned short pattern[2];
  unsigned char attribute[2];
  //unsigned char sprite_pattern[8][2];
}ppu_registers;

typedef struct rendering_sprite {
  unsigned char attribute;
  short x_counter;
  unsigned char sprite_high;
  unsigned char sprite_low;
  unsigned char is_active;
}rendering_sprite;

extern ppu_registers ppu_reg;
extern rendering_sprite sprite[8];

extern char ppu_addr_bus;
extern char is_oddframe;
extern char vram[];

extern char nt_latch;
extern char at_latch;
extern char low_bg_latch;
extern char high_bg_latch;
//extern char sprite_attribute[];
//extern char sprite_x_counter[];

extern char oam[64][4];
extern char second_oam[8][4];