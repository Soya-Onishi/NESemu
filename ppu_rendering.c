#include "ppu.h"

#define MAX_SCANLINE 262
#define MAX_DOTS 341

static int dots = 0;
static int scanline = -1;
static int sprite_count = 0;

void rendering() {
  
  //process


  dots++;
  if(dots == MAX_DOTS) {
    dots = 0;
    scanline = (scanline + 1) % (MAX_SCANLINE);
  }
}

void sprite_process() {
  static int n = 0;
  static int is_disable_writing = 0;

  if(scanline >= 0 && scanline <= 239) {
    if(dots >= 1 && dots <= 32) {
      second_oam[dots - 1] = 0xFF;
    } else if(dots >= 65 && dots <= 256 && n < 64) {
      if(dots % 2 == 0) {
        char y_coordinate = oam[n << 2];

        if(y_coordinate >= scanline + 1 && y_coordinate <= scanline + 9 && sprite_count < 8) {
          short offset = sprite_count << 2;
          short oam_offset = n << 2;

          second_oam[offset] = y_coordinate;
          second_oam[offset + 1] = oam[oam_offset + 1];
          second_oam[offset + 2] = oam[oam_offset + 2];
          second_oam[offset + 3] = oam[oam_offset + 3];
          
          sprite_count++;
        }
        
        n++;
      }
    } else if(dots >= 257 && dots <= 320) {
      int internal_dots;
      int offset;
      unsigned char index, y;

      switch(internal_dots) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
          break;
        case 5:
          offset = (dots - 257) / 8;
          internal_dots = (dots - 257) % 8;

          index = second_oam[offset + 1];
          y = second_oam[offset];

          index = /*pattern table left or right*/ | ((unsigned short)index << 4);
          index += scanline - y;
          
          ppu_reg.sprite_pattern[offset][0] = vram[index];
          ppu_reg.sprite_pattern[offset][1] = vram[index + 8];

          sprite_attribute[offset] = second_oam[offset + 2];
          sprite_x_counter[offset] = second_oam[offset + 3];
          break;
        case 7:
          break;
      }
    }
  } else if(scanline == 261) {

  } 
}

void background_process() {

}

void rendering_process() {
  unsigned char fine_x = ppu_reg.fine_x;
  unsigned char offset;
  unsigned char bg_index, bg_color;

  offset = 1 << fine_x;
  bg_index = ((ppu_reg.pattern[0] & offset) >> offset) | ((ppu_reg.pattern[1] & offset) >> (offset - 1));
  bg_index &= 0x0003;

  bg_color = ((ppu_reg.attribute[0] & offset) >> (offset - 2)) | (ppu_reg.attribute[1] & offset) >> (offset - 3)) | bg_index; 
  bg_color &= 0x000F;
}

void pre_render_scanline() {
  if(dots == 1) {
    //vblank, sprite 0, sprite overflow flags is cleared
  }


}

void visible_scanline() {

  if(scanline == 0 && dots == 0 && is_oddframe) {
    dots = 1;
  }

  if(dots >= 1 && dots <= 256 or dots >= 321 && dots <= 340) {
    int internal_dots;
    int offset;
    unsigned short addr;

    if(dots >= 321) {
      internal_dots = dots - 320;
    } else {
      internal_dots = dots;
    }

    offset = internal_dots % 8;

    switch(offset) {
      case 1:
        if(dots >= 9) {
          ppu_reg.pattern[0] = (ppu_reg.pattern[0] & 0x00FF) ((unsigned short)low_bg_latch << 8);
          ppu_reg.pattern[1] = (ppu_reg.pattern[1] & 0x00FF) ((unsigned short)high_bg_latch << 8);
        }
      case 3:
      case 5:
      case 7:
        break;
      case 2:
        addr = 0x2000 | (ppu_reg.v & 0x0FFF);
        nt_latch = vram[addr];
        break;
      case 4:
        unsigned char x, y, shift;
        addr = 0x23C0 | (ppu_reg.v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
        
        x = (ppu_reg.v & 0x0002) >> 1;
        y = (ppu_reg.v & 0x0040) >> 5;
        shift = y | x;
        
        at_latch = vram[addr] >> (3 - shift);
        break;
      case 6:
        addr = ((unsigned short)nt_latch << 4) & 0x0FF0 + ((ppu_reg.v >> 12) & 0x0007);
        low_bg_latch = vram[addr];
        break;
      case 0;
        addr = ((unsigned short)nt_latch << 4) & 0x0FF0 + ((ppu_reg.v >> 12) & 0x0007);
        high_bg_latch = vram[addr + 8];
        if(dots == 256) {
          if((ppu_reg.v & 0x7000) != 0x7000) {
            ppu_reg.v += 0x1000;
          } else {
            int y;

            ppu_reg.v &= ~0x7000;
            y = (v & 0x03E0) >> 5;
            if(y == 29) {
              y = 0;
              v ^= 0x0800;
            } else if(y == 31) {
              y = 0;
            } else {
              y += 1;
            }

            ppu_reg.v = (ppu_reg.v & ~0x03E0) | (y << 5);
          }
        } else {
          ppu_reg.v++;
        }
        break;
    }

    dots++;
    return;
  }

  if(dots >= 257 && dots <= 320) {
    int offset;
    
    if(dots == 257) {
      ppu_reg.v = (ppu_reg.v & 0xFFE0) | (ppu_reg.t & 0x001F);
    }

    offset = (dots - 256) % 8;
    switch(offset) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 7:
        break;
      case 6:
        //fetch sprite low tile byte
      case 0:
        //fetch sprite high tile byte
    }

    dots++;
    return;
  }
}

void post_render_scanline() {
  //nothing to do
}

void vertical_blank_scanline() {

  if(scanline == 241 && dots == 1) {
    //set vblank flag
  }
}