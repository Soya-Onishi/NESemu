#include "ppu.h"
#include "ppu_rendering.h"

void sprite_visible();
void sprite_pre_render();

void sprite_evaluation();
void sprite_fetch(int dots);

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

  if(ppu_reg.mask & SPRITE_ENABLE) return;

  if(dots >= 1 && dots <= 64) {
    //clear secondary oam
    second_oam[dots >> 2][dots & 3] = 0xFF;
  } else if(dots >= 65  && dots <= 256) {
    sprite_evaluation();
  } else if(dots >= 257 && dots <= 320) {
    //fetch sprites
    ppu_reg.oamaddr = 0;
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
  static int writing_disable = 0;
  int range = 8;
  int scanline = get_scanline();
  int dots = get_dots();

  if(writing_disable) {
    if(dots == 256) {
      writing_disable = 0;
    }

    return;
  }

  if(dots % 2 == 0) {
    if(ppu_reg.ctrl & SPRITE_SIZE) {
      //renewing range for 8*16 mode
      range = 16;
    }

    //step 1 in evaluation 
    if(oam[n][0] <= scanline && oam[n][0] + range > scanline) {
      int i;

      if(n == 0) {
        sprite_zero_exist = 1;
      }

      if(offset == 8) {
        //step 3 in evaluation
        ppu_reg.status |= SPRITE_OVERFLOW;
        n = (n + 1) & 0x3F;
        return;
      }

      for(i = 0; i < 4; i++) {
        second_oam[offset][i] = oam[n][i];
      }

      offset++;
    } else {
      if(offset == 8) {
        n = (n + 1) & 0x3F; // <= n = (n + 1) % 64
        m = (m + 1) & 3;    // <= m = (m + 1) % 4
        if(n == 0) {
          writing_disable = 1;
          return;
        }
      }
    }

    //step 2 in evaluation
    n = (n + 1) & 0x3F;
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

  switch(internal_dots) {
    case 5:
      //fetch low sprite tile byte
      if(ppu_reg.ctrl & SPRITE_SIZE) {
        //TODO:8*16 mode
        
      } else {
        //8*8 mode
        addr = (((unsigned short)ppu_reg.ctrl & (1 << 3)) << 9) | ((unsigned short)second_oam[offset][1] << 4);

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

      if(offset == 0 && sprite_zero_exist) {
        sprite[offset].is_sprite_zero = 1;
      } else {
        sprite[offset].is_sprite_zero = 0;
      }
      
      break;
    default:
      break;
  }
}