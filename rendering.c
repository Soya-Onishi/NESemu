#include "GL/glut.h"
#include "ppu.h"
#include "ppu_rendering.h"
#include "cpu.h"

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

    if((~ppu_reg.mask & SPRITE_MASK) && dots >= 1 && dots <= 8) {
      sprite_pallet = 0;
    }
    if((~ppu_reg.mask & BG_MASK) && dots >= 1 && dots <= 8) {
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
      
      color_addr = vram_read(rendering_addrs[dots][i]);      
      //convert to grayscale color address, if grayscale mode is enable
      if(ppu_reg.mask & GRAYSCALE) {
        color_addr &= 0x30;
      }

      rendering_color[dots][i] = (int *)pallet_colors[color_addr];
    }
  } else if(dots == 240) {
    ready_for_drawing = 1;
  }
}

void display() {
  int x, y;
  const int WIDTH = 256, HEIGHT = 240;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  glBegin(GL_POINTS);
  for(y = 0; y < HEIGHT; y++) {
    for(x = 0; x < WIDTH; x++) {
      GLfloat red, blue, green;

      red = (GLfloat)rendering_color[y][x][0] / 255.0;
      green = (GLfloat)rendering_color[y][x][1] / 255.0;
      blue = (GLfloat)rendering_color[y][x][2] / 255.0;
      
      glColor3f(red, green, blue);
      glVertex2i(x, y);
    }
  }
  glEnd();

  glutSwapBuffers();
  return;
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

        sprite[i].sprite_low  = (sprite[i].sprite_low  >> 1) & 0x7F;
        sprite[i].sprite_high = (sprite[i].sprite_high >> 1) & 0x7F;

        return_offset = i;
      }
    } else {
      sprite[i].x_counter--;
    }
  }

  *addr = sprite_addr;

  if(sprite_addr) {
    return sprite[return_offset];
  } else {
    return std_sprite;
  }
}