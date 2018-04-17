#include "ppu.h"
#include "bg_render.h"
#include "sprite_render.h"
#include "ppu_rendering.h"
#include "rendering.h"

#define MAX_SCANLINE 262
#define MAX_DOTS 341

#define BG_OFFSET 0
#define SPRITE_OFFSET 1
#define RENDER_OFFSET 2
#define SHIFT_REG_OFFSET 3

void nothing_to_do();

static int dots = 0;
static int scanline = 261;
static int frame = 1;

static int true_dots = 0;
static int true_scanline = 261;
static int true_frame = 1;

static int remain_cycle = 0;

static void (*rendering_funcs[MAX_SCANLINE][MAX_DOTS][4])();

void init_rendering_funcs() {
  int scanline, dot;
  int i;

  for(scanline = 0; scanline < MAX_SCANLINE; scanline++) {
    for(dot = 0; dot < MAX_DOTS; dot++) {
      for(i = 0; i < 4; i++) {
        rendering_funcs[scanline][dot][i] = nothing_to_do;
      }
    }
  }

  for(scanline = 0; scanline < MAX_SCANLINE; scanline++) {
    for(dot = 0; dot < MAX_DOTS; dot++) {
      if(scanline >= 0 && scanline <= 239) {
        if((dot >= 1 && dot <= 256) || (dot >= 321 && dot <= 336)) {
          int internal = (dot - 1) % 8;

          switch(internal) {
            case 1:
              rendering_funcs[scanline][dot][BG_OFFSET] = nametable_fetch;
              break;
            case 3:
              rendering_funcs[scanline][dot][BG_OFFSET] = attribute_fetch;
              break;
            case 5:
              rendering_funcs[scanline][dot][BG_OFFSET] = low_byte_tile_fetch;
              break;
            case 7:
              rendering_funcs[scanline][dot][BG_OFFSET] = high_byte_tile_fetch;
              break;
          }
        } else if(dot == 257) {
          rendering_funcs[scanline][dot][BG_OFFSET] = assign_horizontal_loopy_v;
        }
      } else if(scanline == 241) {
        //vblank
        if(dot == 1) {
          rendering_funcs[scanline][dot][BG_OFFSET] = vblank_set;
        }        
      } else if(scanline == 261) {
        //prerender
        if((dot >= 1 && dot <= 256) || (dot >= 321 && dot <= 336)) {
          int internal;

          if(dot == 1) {
            rendering_funcs[scanline][dot][BG_OFFSET] = clear_some_flags;
          }

          internal = (dot - 1) % 8;
          switch(internal) {
            case 1: 
              rendering_funcs[scanline][dot][BG_OFFSET] = nametable_fetch;
              break;
            case 3:
              rendering_funcs[scanline][dot][BG_OFFSET] = attribute_fetch;
              break;
            case 5:
              rendering_funcs[scanline][dot][BG_OFFSET] = low_byte_tile_fetch;
              break;
            case 7:
              rendering_funcs[scanline][dot][BG_OFFSET] = high_byte_tile_fetch;
              break;
          }
        } else if(dot == 257) {
          rendering_funcs[scanline][dot][BG_OFFSET] = assign_horizontal_loopy_v;
        } else if(dot >= 280 && dot <= 304) {
          rendering_funcs[scanline][dot][BG_OFFSET] = assign_vertical_loopy_v;
        }
      }
    }
  }

  for(scanline = 0; scanline < MAX_SCANLINE; scanline++) {
    for(dot = 0; dot < MAX_DOTS; dot++) {
      if(scanline >= 0 && scanline <= 239) {
        if(dot == 1) {
          rendering_funcs[scanline][dot][SPRITE_OFFSET] = initialize_second_oam;    
        } else if(dot >= 65 && dot <= 256) {
          rendering_funcs[scanline][dot][SPRITE_OFFSET] = sprite_evaluation;          
        } else if(dot >= 257 && dot <= 320) {
          rendering_funcs[scanline][dot][SPRITE_OFFSET] = reset_oamaddr_and_fetch_sprite;
        }
      } else if(scanline == 261) {
        if(dot >= 257 && dot <= 320) {
          rendering_funcs[scanline][dot][SPRITE_OFFSET] = reset_oamaddr_and_fetch_sprite;
        }
      }  
    }
  }

  //rendering functions
  for(scanline = 0; scanline < MAX_SCANLINE; scanline++) {
    for(dot = 0; dot < MAX_DOTS; dot++) {
      if(scanline >= 0 && scanline <= 239) {
        if(dot >= 1 && dot <= 256) {
          rendering_funcs[scanline][dot][RENDER_OFFSET] = decide_render_addr;
        }
      } else if(scanline == 240) {
        if(dot == 1) {
          rendering_funcs[scanline][dot][RENDER_OFFSET] = set_ready_rendering;
        }
      }
    }
  }

  for(scanline = 0; scanline < MAX_SCANLINE; scanline++) {
    for(dot = 0; dot < MAX_DOTS; dot++) {
      if((scanline >= 0 && scanline <= 239) || scanline == 261) {
        if(dot >= 1 && dot <= 336) {
          if(dot >= 9 && (dot & 7) == 1) {
            rendering_funcs[scanline][dot][SHIFT_REG_OFFSET] = reload_and_shift_reg;
          } else {
            rendering_funcs[scanline][dot][SHIFT_REG_OFFSET] = shift_reg;
          }
        } else if(dot == 337) {
          rendering_funcs[scanline][dot][SHIFT_REG_OFFSET] = reload_shift_reg;
        }
      }
    }
  }
}

void ppu_rendering() {
  
  
  if(true_scanline == 0 && true_dots == 0 && true_frame % 2 == 1 && (~ppu_reg.mask & BG_ENABLE) && (~ppu_reg.mask & SPRITE_ENABLE)) {
    true_dots++;
  }

  ppu_cycle_number++;
  remain_cycle++;
  
  if(true_scanline == 241 && true_dots == 2) {
    start_rendering();
  }

  true_dots++;
  if(true_dots == MAX_DOTS) {
    true_dots = 0;
    true_scanline = (true_scanline + 1) % (MAX_SCANLINE);

    if(true_scanline == 0) {
      true_frame++;
    }
  }
}

void start_rendering() {
  void (*bg_rendering)(), (*sprite_rendering)(), (*render)(), (*reg_manage)();

  for(; remain_cycle > 0; remain_cycle--) {
    if(scanline == 0 && dots == 0 && (frame & 1) == 1) {
      dots++;
    }

    bg_rendering = rendering_funcs[scanline][dots][BG_OFFSET];
    sprite_rendering = rendering_funcs[scanline][dots][SPRITE_OFFSET];
    render = rendering_funcs[scanline][dots][RENDER_OFFSET];
    reg_manage = rendering_funcs[scanline][dots][SHIFT_REG_OFFSET];

    bg_rendering();
    sprite_rendering();
    render();
    reg_manage();

    dots++;
    if(dots == MAX_DOTS) {
      dots = 0;
      scanline = (scanline + 1) % MAX_SCANLINE;

      if(scanline == 0) {
        frame++;
      }
    }
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

int get_true_scanline() {
  return true_scanline;
}

int get_true_dots() {
  return true_dots;
}

void nothing_to_do() {
  //nothing to do
}