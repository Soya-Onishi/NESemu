#ifndef HEADER_PPU
#define HEADER_PPU

typedef struct ppu_registers {
  unsigned short v;
  unsigned short t;
  unsigned char toggle;
  unsigned char fine_x;
  unsigned short pattern[2];
  unsigned char attribute[2];
  unsigned char sprite_pattern[8][2];
}ppu_registers;

extern ppu_registers ppu_reg;

extern char ppu_addr_bus;
extern char is_oddframe;
extern char vram[];

extern char nt_latch;
extern char at_latch;
extern char low_bg_latch;
extern char high_bg_latch;
extern char sprite_attribute[];
extern char sprite_x_counter[];

extern char oam[];
extern char second_oam[];