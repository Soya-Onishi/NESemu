#include "ppu.h"
#include "ppu_rendering.h"

void rendering() {
  int scanline = get_scanline();
  int dots = get_dots();
  unsigned short bg_addr, sprite_addr, addr;
  unsigned char bg_pallet, sprite_pallet;
  sprite sp;

  if(scanline >= 0 && scanline <= 239) {
    bg_addr = calc_bg_pixel_addr();
    sp = calc_sprite_pixel_addr(&sprite_addr);

    bg_pallet = bg_addr & 3;
    sprite_pallet = sprite_addr & 3;

    if(bg_pallet == 0 && sprite_pallet == 0) {
      addr = 0x3F00;
    } else if(bg_pallet == 0 && sprite_pallet != 0) {
      addr = sprite_addr;
    } else if(bg_pallet != 0 && sprite_pallet == 0) {
      addr = bg_addr;
    } else if(sp.attribute & BG_PRIO) {
      addr = bg_addr;
    } else {
      addr = sprite_addr;
    }

    //store addr;
    if(dots <= 256) {
      rendering_addrs[scanline][dots - 1] = addr;
    }
  }
}

void renew_registers() {
  int scanline = get_scanline();
  int dots = get_dots();
  int i;

  if((scanline >= 0 && scanline <= 239) || scanline == -1 || scanline == 261) {
    if(dots >= 1 && dots <= 336) {
      if(dots >= 9 && dots % 8 == 1) {
        reload_shift_reg();
      }
      
      for(i = 0; i < 2; i++) {
        bg_pattern_reg[i] >>= 1;
        bg_attr_reg[i] >>= 1;
      }
    } else if(dots == 337) {
      reload_shift_reg();
    }
  }
}

void reload_shift_reg() {
  int i;

  for(i = 0; i < 2; i++) {
    bg_pattern_reg[i] = (bg_pattern_reg[i] & 0x0F) | ((unsigned short)bg_latch[i] << 8);
    bg_attr_reg[i] = (bg_attr_reg[i] & 0x0F) | ((unsigned short)attribute_nexttile << 8);
  }
}

unsigned short calc_bg_pixel_addr() {
  unsigned short bg_addr, sprite_addr;
  unsigned char fine_x = ppu_render_info.fine_x;

  bg_addr = ((attribute_reg[0] & (1 << fine_x)) << (2 - fine_x)) | ((attribute_reg[1] & (1 << fine_x)) << (3 - fine_x));
  bg_addr |= ((bg_pattern_reg[0] & (1 << fine_x)) << (0 - fine_x)) | ((bg_pattern_reg[1] & (1 << fine_x)) << (1 - fine_x));
  bg_addr &= 0x000F;
  bg_addr |= 0x3F00;

  return bg_addr;
}

sprite calc_sprite_pixel_addr(unsigned short *addr) {
  unsigned short sprite_addr;
  unsigned char fine_x = ppu_render_info.fine_x;
  int i;
  sprite std_sprite = {0xFF, 0xFF, 0xFF, 0xFF};

  for(i = 0; i < 8; i++) {
    if(sprite[i].x_counter <= 0 && sprite[i].x_counter > -8) {
      int offset;

      if(sprite[i].attribute & H_REV) {
        offset = 7 + sprite[i].x_counter;
      } else {
        offset = sprite[i].x_counter;
      }

      sprite_addr = (sprite[i].sprite_high & (1 << offset) << (1 - offset)) | (sprite[i].sprite_low & (1 << offset) << (0 - offset));
      if(sprite_addr == 0) {
        sprite[i].x_counter--;
        continue;
      }

      sprite_addr |= ((sprite[i].attribute & SPRITE_PALLET) << 2);

      for(; i < 8; i++) {
        sprite[i].x_counter--;
      }

      *addr = sprite_addr;
      return sprite[i];
    }

    sprite[i].x_counter--;
  }

  *addr = 0;
  return std_sprite;
}