#include <stdio.h>
#include "GL/gl.h"
#include "GL/glut.h"
#include "ppu.h"
#include "ppu_rendering.h"
#include "cpu.h"
#include "window.h"

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

  if(scanline >= 0 && scanline <= 239) {
    //select displayed pixel's address
    if(dots >= 1 && dots <= 256) {
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
      renew_registers();

      partical_drawing(addr);
    } else if(dots >= 321 && dots <= 337) {
      renew_registers();
    }

    /*
    display display's color addr

    if((addr & 0xF) == 0) {
      printf("   ");
    } else {
      printf("%d%d ", (addr & 12) >> 2, addr & 3);
    }
    if(dots == 256) {
      printf("\n");
      if(scanline == 239) {
        printf("\n=========================================\n\n");
      }
    }
    */
    
  } else if(scanline == 240) {
    //address_to_color();
    if(dots == 1) {
      ready_for_drawing = 1;
    }
  /*
    int x, y;

    for(y = 0; y < 30; y++) {
      for(x = 0; x < 32; x++) {
        printf("%2x ", vram[0x2000 + (y * 32) + x]);
      }
      printf("\n");
    }
    printf("\n");
    */

   /*
   int x, y;

   for(y = 0; y < 8; y++) {
      for(x = 0; x < 8; x++) {
      unsigned char lower = vram[0x480 + y] & (1 << x);
      unsigned char upper = vram[0x488 + y] & (1 << x);
      unsigned char data = 0;

      if(lower) {
        data = 1;
      }
      if(upper) {
        data |= 2;
      }
      
      printf("%2d", data);
     }
     printf("\n");
   }
   printf("\n");
   */

    /*
    //display oam content
    int n, m;
    for(n = 0; n < 64; n++) {
      for(m = 0; m < 4; m++) {
        printf("%02x ", oam[n][m]);
      }
      printf("\n");
    }
    printf("\n===========\n\n");
    */

    /*
    //display palette
    int p;
    for(p = 0x3F00; p < 0x3F20; p++) {
      printf("%02X ", vram[p]);
      if(p % 4 == 3) {
        printf("\n");
      } 
    }
    printf("\n==========================\n\n");
    */
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
  int i, j;

  if(dots < 240) {
    for(i = 0; i < 256; i++) {
      unsigned char color_addr;
      
      color_addr = vram_read(rendering_addrs[dots][i]);      
      //convert to grayscale color address, if grayscale mode is enable
      if(ppu_reg.mask & GRAYSCALE) {
        color_addr &= 0x30;
      }

      for(j = 0; j < 3; j++) {
        rendering_color[dots][i][j] = palette_colors[color_addr][j];
      }

      /*
      output palette data per frame

      int p;
      for(p = 0; p < 0x20; p++) {
        printf("%02x ", vram[0x3F00 + p]);
        if(p % 4 == 3) {
          printf("\n");
        } 
      }
      printf("\n=================================\n\n");
      */

      //printf("%02x ", color_addr & 0xFF);
    }
    //printf("\n");

  } else if(dots == 240) {
    ready_for_drawing = 1;
    //printf("\n======================================================\n\n");
  }
}

/*
void display() {
  int x, y;
  const int WIDTH = 256, HEIGHT = 240;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  
  glPointSize(10);
  glBegin(GL_POINTS);
  for(y = 8; y < HEIGHT - 8; y++) {
    for(x = 0; x < WIDTH; x++) {
      GLfloat red, blue, green;
      
      red = (GLfloat)rendering_color[y][x][0];
      green = (GLfloat)rendering_color[y][x][1];
      blue = (GLfloat)rendering_color[y][x][2];
      
      glColor3f(red, green, blue);
      glVertex2i(x, y);
    }
  }
  glEnd();

  glutSwapBuffers();
  return;
}
*/

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

  
  /*
  {
    int scanline = get_scanline();

    printf("%d", bg_addr & 0x3);
    if((dots - 1) % 8 == 7) {
      printf(" ");
    }

    if(dots == 256) {
      printf("\n");
      if(scanline % 8 == 7) {
        printf("\n");
      }
      if(scanline == 239) {
        printf("\n");
      }
    }
    
    
  }
  */
 
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
