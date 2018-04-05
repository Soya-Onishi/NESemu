#include "ppu.h"
#include "ppu_rendering.h"

void bg_tile_fetch(int dots);

void bg_render() {
  int scanline = get_scanline();

  if(scanline == -1 || scanline == 261) {
    bg_pre_render();
  } else if(scanline >= 0 && scanline <= 239) {
    bg_visible_frame();
  } else if(scanline == 240) {
    bg_post_rendering();
  } else if(scanline >= 241 && scanline <= 260) {
    bg_vblank();
  }
}

void bg_visible_frame() {
  int dots = get_dots();

  if((dots >= 1 && dots <= 256) || (dots >= 321 && dots <= 336) {
    bg_tile_fetch(dots);
  }
}

void bg_post_rendering() {
  //nothing to do
}

void bg_vblank() {
  if(get_scanline() == 241 && get_dots() == 1) {
    ppu_reg.status |= START_VBLANK;
  }
}

void bg_pre_render() {
  int dots = get_dots();
  unsigned short addr;

  if(dots == 1) {
    //clear vblank, sprite 0 hit, sprite overflow flag
    ppu_reg.status &= ~(START_VBLANK | SPRITE_HIT | SPRITE_OVERFLOW);
  }

  if((dots >= 1 && dots <= 256) || (dots >= 321 && dots <= 336) {
    bg_tile_fetch(dots);
  } else if(dots == 257) {
    //copy horizontal of t to horizontal of v
    ppu_render_info.v = (ppu_render_info.v & ~0x001F) | (ppu_render_info.t & 0x001F);
  } else if (dots >= 280 && dots <= 304) {
    ppu_render_info.v = (ppu_render_info.v & ~0x73E0) | (ppu_render_info.t & 0x73E0);
  }
}

void bg_tile_fetch(int dots) {
  int internal_dots = (dots - 1) % 8;

    switch(internal_dots) {
      case 0:
      case 2:
      case 4:
      case 6:
        break;
      case 1:
        //read byte from nametable
        addr = 0x2000 | (ppu_render_info.v & 0x0FFF);
        nt_latch = vram[addr];
        break;
      case 3:
        //read byte from attribute table
        int x, y;

        addr = 0x23C0 | (ppu_render_info.v & 0x0C00) | ((ppu_render_info.v >> 4) & 0x38) | ((v >> 2) & 0x07);
        x = (ppu_render_info.fine_x >> 1) & 1;
        y = (ppu_render_info.v >> 12) & 2;
        
        at_latch = vram[addr] >> ((x | y) << 2);
        break;
      case 5:
        //read low bg tile byte
        addr = ((unsigned short)ppu_reg.ctrl & (1 << 4)) << 8 | (nt_latch << 4);
        low_bg_latch = vram[addr]; 
        break;
      case 7:
        //read high bg tile byte and increment horizontal or vertical of v
        addr = ((unsigned short)ppu_reg.ctrl & (1 << 4)) << 8 | (nt_latch << 4);
        high_bg_latch = vram[addr + 8];

        if(dots == 256) {
          //increment vertical of v
          if((ppu_render_info.v & 0x7000) != 0x7000) {
            ppu_render_info.v += 0x1000;
          } else {
            int y;

            ppu_render_info.v &= ~0x7000;
            y = (ppu_render_info.v & 0x3E0) >> 5;

            if(y == 29) {
              y = 0;
              ppu_render_info.v ^= 0x0800;
            } else if(y == 31) {
              y = 0;
            } else {
              y++;
            }

            ppu_render_info.v = (v & ~0x03E0) | (y << 5);
          }
        } else {
          //increment horizontal of v
          if((ppu_render_info.v & 0x001F) == 31) {
            ppu_render_info.v &= ~0x001F;
            ppu_render_info.v ^= 0x0400;
          } else {
            ppu_render_info.v++;
          }
        }
        break;
    }
}
