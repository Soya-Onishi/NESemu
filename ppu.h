#ifndef HEADER_PPU
#define HEADER_PPU

#define NMI_ENABLE (1 << 7)
#define SPRITE_SIZE (1 << 5)
#define ADDR_INCREMENT (1 << 2)

#define SPRITE_ENABLE (1 << 4)
#define BG_ENABLE (1 << 3)
#define SPRITE_MASK (1 << 2)
#define BG_MASK (1 << 1)
#define GRAYSCALE (1 << 0)

#define START_VBLANK (1 << 7)
#define SPRITE_HIT (1 << 6)
#define SPRITE_OVERFLOW (1 << 5)

#define V_REV (1 << 7)
#define H_REV (1 << 6)
#define BG_PRIO (1 << 5)
#define SPRITE_PALLET (3)


typedef struct ppu_registers {
  unsigned char ctrl;
  unsigned char mask;
  unsigned char status;
  unsigned char oamaddr;
  unsigned char oamdata;
  unsigned char scroll;
  unsigned char ppuaddr;
  unsigned char ppudata;
  unsigned char oamdma;
}ppu_registers;

typedef struct ppu_rendering_reg {
  unsigned short v;
  unsigned short t;
  unsigned char toggle;
  unsigned char fine_x;
}ppu_rendering_reg;

typedef struct rendering_sprite {
  unsigned char attribute;
  short x_counter;
  unsigned char sprite_high;
  unsigned char sprite_low;
  unsigned char is_sprite_zero;
}rendering_sprite;


extern ppu_rendering_reg ppu_render_info;
extern ppu_registers ppu_reg;
extern rendering_sprite sprite[8];

extern unsigned char ppu_addr_bus;
extern unsigned char ppu_data_bus;
extern unsigned char vram[];

extern unsigned char nt_latch;
extern unsigned char at_latch;
extern unsigned char bg_latch[2];

extern unsigned short bg_pattern_reg[2];
extern unsigned short bg_attr_reg[2];
extern unsigned char  sprite_zero_exist;

extern unsigned char oam[64][4];
extern unsigned char second_oam[8][4];

extern unsigned short rendering_addrs[240][256];
extern int *rendering_color[240][256];
extern unsigned char pallet_colors[0x40][3];

void ppu_cycle();

void init_ppu();
void reset_ppu();

void vram_write(unsigned short addr, unsigned char data);
unsigned char vram_read(unsigned short addr);

#endif