#include <stdio.h>
#include "GL/gl.h"
#include "GL/glut.h"
#include "ppu.h"
#include "ppu_rendering.h"
#include "rendering.h"
#include "cpu.h"
#include "window.h"

void renew_registers();
void address_to_color();
void reload_shift_reg();
unsigned short calc_bg_pixel_addr();
rendering_sprite calc_sprite_pixel_addr(unsigned short *addr);


void decide_render_addr() {
  unsigned short bg_addr, sprite_addr, addr;
  unsigned char bg_pallet, sprite_pallet;
  rendering_sprite sp;

  bg_addr = calc_bg_pixel_addr();
  sp = calc_sprite_pixel_addr(&sprite_addr);

  bg_pallet = bg_addr & 3;
  sprite_pallet = sprite_addr & 3;
  
  if(bg_pallet == 0) {
    if(sprite_pallet) {
      //sprite_pallet != 0
      addr = sprite_addr;
    } else {
      //sprite_pallet == 0
      addr = 0x3F00;
    }
  } else {
    if(sprite_pallet) {
      int dots = get_dots();

      if(sp.is_sprite_zero && dots != 256) {
        ppu_reg.status |= SPRITE_HIT;
      }
    
      if(sp.attribute & BG_PRIO) {
        addr = bg_addr;
      } else {  
        addr = sprite_addr;
      }
    } else {
      addr = bg_addr;
    }
  }
  
  rendering_addrs[get_scanline()][get_dots() - 1] = addr;
}

void set_ready_rendering() {
  ready_for_drawing = 1;
}

void rendering() {
  int scanline = get_scanline();
  int dots = get_dots();
  unsigned short bg_addr, sprite_addr, addr;
  unsigned char bg_pallet, sprite_pallet;
  rendering_sprite sp;

  if(scanline >= 0 && scanline <= 239) {
    //select displayed pixel's address
    if(dots >= 1 && dots <= 256) {
      bg_addr = calc_bg_pixel_addr();
      sp = calc_sprite_pixel_addr(&sprite_addr);

      bg_pallet = bg_addr & 3;
      sprite_pallet = sprite_addr & 3;
      
      if(bg_pallet == 0) {
        if(sprite_pallet) {
          //sprite_pallet != 0
          addr = sprite_addr;
        } else {
          //sprite_pallet == 0
          addr = 0x3F00;
        }
      } else {
        if(sprite_pallet) {
          int dots = get_dots();

          if(sp.is_sprite_zero && dots != 256) {
            ppu_reg.status |= SPRITE_HIT;
          }
        
          if(sp.attribute & BG_PRIO) {
            addr = bg_addr;
          } else {  
            addr = sprite_addr;
          }
        } else {
          addr = bg_addr;
        }
      }
      
      rendering_addrs[get_scanline()][get_dots() - 1] = addr;
      renew_registers();
      
    } else if(dots >= 321 && dots <= 337) {
      renew_registers();
    }
    
  } else if(scanline == 240) {
    //address_to_color();
    if(dots == 1) {
      ready_for_drawing = 1;
    }
  }
}

void reload_and_shift_reg() {
  reload_shift_reg();
  shift_reg();
}

void shift_reg() {
  int i;
  
  for(i = 0; i < 2; i++) {
    bg_pattern_reg[i] >>= 1;
    bg_attr_reg[i] >>= 1;
  }
}

void renew_registers() {
  int scanline = get_scanline();
  int dots = get_dots();
  int i;

  if((scanline >= 0 && scanline <= 239) || scanline == -1 || scanline == 261) {
    if(dots >= 1 && dots <= 336) {
      if(dots >= 9 && ((dots & 7) == 1)) {
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
    bg_pattern_reg[i] = (bg_pattern_reg[i] & 0xFF) | ((unsigned short)bg_latch[i] << 8);
    bg_attr_reg[i] = (bg_attr_reg[i] & 0xFF);
    
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
  unsigned short sprite_addr = 0;
  int i, return_offset = 7;
  rendering_sprite std_sprite = {0x0, 0x0, 0x0, 0x0, 0};

  if((~ppu_reg.mask & SPRITE_MASK) && get_dots() < 9) {
    *addr = 0;
    return std_sprite;
  }

  if(~ppu_reg.mask & SPRITE_ENABLE) {
    *addr = 0;
    return std_sprite;
  }

  for(i = 0; i < 8; i++) {
    if(sprite[i].x_counter == 0) {
      if(!sprite_addr) {
        sprite_addr = sprite[i].sprite_low & 0x01;
        sprite_addr |= (sprite[i].sprite_high & 0x01) << 1;

        return_offset = i;
      }

      sprite[i].sprite_low  = (sprite[i].sprite_low  >> 1) & 0x7F;
      sprite[i].sprite_high = (sprite[i].sprite_high >> 1) & 0x7F;
    } else {
      sprite[i].x_counter--;
    }
  }

  if(sprite_addr) {
    *addr = 0x3F10 | ((sprite[return_offset].attribute & 0x03) << 2) | sprite_addr;
    return sprite[return_offset];
  } else {
    *addr = 0;
    return std_sprite;
  }
}
