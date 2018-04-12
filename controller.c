#include<stdio.h>
#include "GL/glut.h"
#include"cpu_circuit.h"
#include"controller.h"
#include"ppu.h"

static unsigned char p1_internal_register = 0;
static unsigned char p2_internal_register = 0;
static unsigned char p1_serve_register = 0;
static unsigned char p2_serve_register = 0;
static unsigned char reloading = 0;

void set_controller(unsigned char data) {
  reloading = data & 0x1;
  
  if(reloading == 0) {
    p1_serve_register = p1_internal_register;
  }
}

unsigned char read_controller(unsigned short addr) {
  unsigned char data;
  
  if(addr == 0x4016) {
    if(reloading) {
      return p1_internal_register & 0x01;
    } else {
      data = p1_serve_register & 0x01;
      p1_serve_register >>= 1;
      return data;
    }
  } else {
    if(reloading) {
      return p2_internal_register & 0x01;
    } else {
      data = p2_serve_register & 0x01;
      p2_serve_register >>= 1;
      return data;
    }
  }
}

void display_nametable() {
  //display nametable $2000~ and $2400~
    int p;

    for(p = 0x2000; p < 0x23C0; p++) {
      if(vram[p] != 0) {
        printf("%2X ", vram[p]);
      } else {
        printf("   ");
      }
      if((p - 0x2000) % 32 == 31) {
        printf("\n");
      }
    }
    printf("\n");

    for(p = 0x2400; p < 0x27C0; p++) {
      if(vram[p] != 0) {
        printf("%2X ", vram[p]);
      } else {
        printf("   ");
      }
      if((p - 0x2400) % 32 == 31) {
        printf("\n");
      }
    }

    printf("\n=================================================================\n\n");
}

void key_down(unsigned char key, int x, int y) {
  switch(key) {
    case 'a':
    case 'A':
      p1_internal_register |= CONTROLLER_A;
      break;
    case 'b':
    case 'B':
      p1_internal_register |= CONTROLLER_B;
      break;
    case 'p':
    case 'P':
      p1_internal_register |= CONTROLLER_START;
      break;
    case 'o':
    case 'O':
      p1_internal_register |= CONTROLLER_SELECT;
      break;
    case 'm':
      //display_nametable();
      break;
  }
}

void key_up(unsigned char key, int x, int y) {
  switch(key) {
    case 'a':
    case 'A':
      p1_internal_register &= ~CONTROLLER_A;
      break;
    case 'b':
    case 'B':
      p1_internal_register &= ~CONTROLLER_B;
      break;
    case 'p':
    case 'P':
      p1_internal_register &= ~CONTROLLER_START;
      break;
    case 'o':
    case 'O':
      p1_internal_register &= ~CONTROLLER_SELECT;
      break;
  }
}

void special_key_down(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_UP:
      p1_internal_register |= CONTROLLER_UP;
      break;
    case GLUT_KEY_DOWN:
      p1_internal_register |= CONTROLLER_DOWN;
      break;
    case GLUT_KEY_LEFT:
      p1_internal_register |= CONTROLLER_LEFT;
      break;
    case GLUT_KEY_RIGHT:
      p1_internal_register |= CONTROLLER_RIGHT;
      break;
  }
}

void special_key_up(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_UP:
      p1_internal_register &= ~CONTROLLER_UP;
      break;
    case GLUT_KEY_DOWN:
      p1_internal_register &= ~CONTROLLER_DOWN;
      break;
    case GLUT_KEY_LEFT:
      p1_internal_register &= ~CONTROLLER_LEFT;
      break;
    case GLUT_KEY_RIGHT:
      p1_internal_register &= ~CONTROLLER_RIGHT;
      break;
  }
}