#include "ppu.h"
#include "ppu_rendering.h"

void renew_registers();
void address_to_color();
void reload_shift_reg();
unsigned short calc_bg_pixel_addr();
rendering_sprite calc_sprite_pixel_addr(unsigned short *addr);


void rendering() {
  int scanline = get_scanline();
  int dots = get_dots();
  unsigned short bg_addr, sprite_addr, addr;
  unsigned char bg_pallet, sprite_pallet;
  rendering_sprite sp;

  if(scanline >= 0 && scanline <= 239 && dots >= 1 && dots <= 256) {
    //select displayed pixel's address
    bg_addr = calc_bg_pixel_addr();
    sp = calc_sprite_pixel_addr(&sprite_addr);

    bg_pallet = bg_addr & 3;
    sprite_pallet = sprite_addr & 3;

    if(~ppu_reg.mask & SPRITE_MASK) {
      sprite_pallet = 0;
    }
    if(~ppu_reg.mask & BG_MASK) {
      bg_pallet = 0;
    }

    if(bg_pallet == 0 && sprite_pallet == 0) {
      addr = 0x3F00;
    } else if(bg_pallet == 0 && sprite_pallet != 0) {
      addr = sprite_addr;
    } else if(bg_pallet != 0 && sprite_pallet == 0) {
      addr = bg_addr;
    } else if(sp.attribute & BG_PRIO) {
      if(sp.is_sprite_zero && dots != 256) {
        ppu_reg.status |= SPRITE_HIT;
      }

      addr = bg_addr;
    } else {
      if(sp.is_sprite_zero && dots != 256) {
        ppu_reg.status |= SPRITE_HIT;
      }

      addr = sprite_addr;
    }

    //store addr;
    rendering_addrs[scanline][dots - 1] = addr;
  } else if(scanline == 240) {
    address_to_color();
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

void address_to_color() {
  int dots = get_dots();
  int i;

  if(dots < 240) {
    for(i = 0; i < 256; i++) {
      unsigned char color_addr;
      
      color_addr = vram[rendering_addrs[dots][i]];
      
      //convert to grayscale color address, if grayscale mode is enable
      if(ppu_reg.mask & GRAYSCALE) {
        color_addr &= 0x30;
      }

      rendering_color[dots][i] = (int *)pallet_colors[color_addr];
    }
  } else if(dots == 240) {
    //TODO:display rendering result by openGL
  }
}

void reload_shift_reg() {
  int i;

  for(i = 0; i < 2; i++) {
    bg_pattern_reg[i] = (bg_pattern_reg[i] & 0x0F) | ((unsigned short)bg_latch[i] << 8);
    bg_attr_reg[i] = (bg_attr_reg[i] & 0x0F);
    
    //at_latch       : bit0 = 0 bit1 = 1
    //bg_attr_reg[0] : 0000 0000 1111 1111 => 0000 0000 1111 1111
    //bg_attr_reg[1] : 0000 0000 0000 0000 => 1111 1111 0000 0000
    if(at_latch & (1 << i)) {
      bg_attr_reg[i] |= 0xFF00;
    }
  }
}

unsigned short calc_bg_pixel_addr() {
  unsigned short bg_addr;
  unsigned char fine_x = ppu_render_info.fine_x;
  int dots = get_dots();

  if((~ppu_reg.mask & BG_MASK) && dots < 9) {
    return 0;
  }

  if(~ppu_reg.mask & BG_ENABLE) {
    return 0;
  }

  if(bg_attr_reg[0] & (1 << fine_x)) {
    bg_addr = 1 << 2;
  } else {
    bg_addr = 0;
  }

  if(bg_attr_reg[1] & (1 << fine_x)) {
    bg_addr |= 1 << 3;
  }

  if(bg_pattern_reg[0] & (1 << fine_x)) {
    bg_addr |= 1 << 0;
  }

  if(bg_pattern_reg[1] & (1 << fine_x)) {
    bg_addr |= 1 << 1;
  }

  bg_addr |= 0x3F00;

  return bg_addr;
}

rendering_sprite calc_sprite_pixel_addr(unsigned short *addr) {
  unsigned short sprite_addr;
  unsigned char fine_x = ppu_render_info.fine_x;
  int i;
  rendering_sprite std_sprite = {0xFF, 0xFF, 0xFF, 0xFF};

  if((~ppu_reg.mask & SPRITE_MASK) && get_dots() < 9) {
    *addr = 0;
    return std_sprite;
  }

  if(~ppu_reg.mask & SPRITE_ENABLE) {
    *addr = 0;
    return std_sprite;
  }

  for(i = 0; i < 8; i++) {
    if(sprite[i].x_counter <= 0 && sprite[i].x_counter > -8) {
      int offset;

      if(sprite[i].attribute & H_REV) {
        offset = 7 + sprite[i].x_counter;
      } else {
        offset = sprite[i].x_counter;
      }

      sprite_addr = (sprite[i].sprite_high & (1 << offset) << (1 - offset)) | (sprite[i].sprite_low & (1 << offset) << (0 - offset));
      if(sprite[i].sprite_high & (1 << offset)) {
        sprite_addr = 1 << 1;
      } else {
        sprite_addr = 0;
      }

      if(sprite[i].sprite_low & (1 << offset)) {
        sprite_addr |= 1 << 0;
      }

      if(sprite_addr == 0) {
        sprite[i].x_counter--;
        continue;
      }

      sprite_addr |= ((sprite[i].attribute & SPRITE_PALLET) << 2);

      for(; i < 8; i++) {
        sprite[i].x_counter--;
      }

      sprite_addr |= 0x3F10;
      *addr = sprite_addr;
      return sprite[i];
    }

    sprite[i].x_counter--;
  }

  *addr = 0;
  return std_sprite;
}