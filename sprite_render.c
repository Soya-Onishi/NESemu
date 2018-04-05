#include "ppu.h"
#include "ppu_rendering.h"

void sprite_render() {
  int scanline = get_scanline();

  if(scanline >= 0 && scanline <= 239) {
    sprite_visible();
  } else if(scanline == -1 || scanline == 261){
    sprite_pre_render();
  }
}

void sprite_visible() {
  int dots = get_dots();
  static int writing_disable = 0;

  if(writing_disable) return;

  if(dots >= 1 && dots <= 64) {
    //clear secondary oam
    *(second_oam + (dots - 1)) = 0xFF;
  } else if(dots >= 65  && dots <= 256) {
    sprite_evaluation();
  } else if(dots >= 257 && dots <= 320) {
    //fetch sprites
    sprite_fetch(dots);
  }
}

void sprite_pre_render() {
  int dots = get_dots();

  if(dots >= 257 && dots <= 320) {
    //fetch sprites
    sprite_fetch(dots);
  }
}

void sprite_evaluation() {
  //sprite evaluation for next scanline
  static int n = 0;
  static int m = 0;
  static int offset = 0;
  int range = 8;
  int scanline = get_scanline();

  if(dots % 2 == 0) {
    if(ppu_reg.v & SPRITE_SIZE) {
      //renewing range for 8*16 mode
      range = 16;
    }

    //step 1 in evaluation 
    if(oam[n][0] <= scanline && oam[n][0] + range > scanline) {
      int i;

      if(offset == 8) {
        //step 3 in evaluation
        ppu_reg.status |= SPRITE_OVERFLOW;
        n = (n + 1) % 64;
        return;
      }

      for(i = 0; i < 4; i++) {
        second_oam[offset][i] = oam[n][i];
      }
      offset++;
    } else {
      if(offset == 8) {
        n = (n + 1) % 64;
        m = (m + 1) % 4;
        if(n == 0) {
          writing_disable = 1;
          return;
        }
      }
    }

    //step 2 in evaluation
    n = (n + 1) % 64;
    if(offset < 8) return;
    if(n == 0) {
      writing_disable = 1;
      return;
    }
  }
}

void sprite_fetch(int dots) {
  int internal_dots = (dots - 257) % 8;
  int offset = (dots - 257) / 8;
  static unsigned short addr;
  const unsigned char V_REV = (1 << 7);
  const unsigned char SPRITE_SIZE = (1 << 5);

  switch(internal_dots) {
    case 5:
      //fetch low sprite tile byte
      if(ppu_reg.ctrl & SPRITE_SIZE) {
        //8*16 mode
        
      } else {
        //8*8 mode
        addr = ((unsigned short)ppu_reg.ctrl & (1 << 3)) << 9 | (unsigned short)second_oam[offset][1] << 4;

        if(second_oam[offset][2] & V_REV) {
          //vertical reverse      
          addr += 7 - get_scanline() - second_oam[offset][0];
        } else {
          //not vertical reverse
          addr += get_scanline() - second_oam[offset][0];
        }  
        sprite[offset].sprite_low = vram[addr];
      }
      break;
    case 7:
      //fetch high sprite tile byte
      sprite[offset].sprite_high = vram[addr + 8];
      break;
    default:
      break;
  }
}