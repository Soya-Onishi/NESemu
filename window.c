#include <stdio.h>
#include "GL/gl.h"
#include "GL/glut.h"
#include "ppu.h"
#include "ppu_rendering.h"
#include "window.h"


#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 240

void convert_palette_address();

static GLubyte texture[WINDOW_HEIGHT * WINDOW_WIDTH * 3];
static GLuint texid;

void init_window(power_on_type type) {
  int i, limit = WINDOW_HEIGHT * WINDOW_WIDTH * 3;
  int x, y;

  for(i = 0; i < limit; i++) {
    texture[i] = 0;
  }

  if(type == WINDOW_RESET) return;

  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &texid);
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

/*
void partical_drawing(unsigned char addr) {
  int x, y, i;

  x = get_dots() - 1;
  y = get_scanline();

  if(y < 8 || y >= 232) return;
  if(vram[addr] == palette_addrs[y][x]) return;

  palette_addrs[y][x] = vram[addr];
  for(i = 0; i < 3; i++) {
    texture[i + x * 3 + y * WINDOW_WIDTH * 3] = palette_colors[palette_addrs[y][x]][i];
  }

  //printf("(%d, %d): %d, %d, %d\n", x, y, texture[0 + x * 3 + y * WINDOW_WIDTH * 3], texture[1 + x * 3 + y * WINDOW_WIDTH * 3], texture[2 + x * 3 + y * WINDOW_WIDTH * 3]);

  glBindTexture(GL_TEXTURE_2D, texid);
  glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, texture);
}
*/

void convert_palette_address() {
  int x, y, i;

  for(y = 8; y < WINDOW_HEIGHT - 8; y++) {
    for(x = 0; x < WINDOW_WIDTH; x++) {
      unsigned char color_number = vram[rendering_addrs[y][x]];
      
      for(i = 0; i < 3; i++) {
        texture[i + x * 3 + y * WINDOW_WIDTH * 3] = palette_colors[color_number][i];
      }
    }
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  convert_palette_address();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex2i(0, 0);
  glTexCoord2f(0, 1); glVertex2i(0, WINDOW_HEIGHT);
  glTexCoord2f(1, 1); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
  glTexCoord2f(1, 0); glVertex2i(WINDOW_WIDTH, 0);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  glutSwapBuffers();
}