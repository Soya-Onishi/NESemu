#include<stdio.h>
#include"GL/glut.h"
#include"cpu.h"
#include"ppu.h"
#include"memory.h"
#include"rendering.h"

#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 240

void reshape(int w, int h);
void open_file(char *filename);

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("write filename\n");
    exit(1);
  }

  glutInit(&argc, argv);
  glutInitWindowSize(WINDOW_WIDTH * 3, WINDOW_HEIGHT * 3);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutCreateWindow("NES EMU");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(cpu);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  init_cpu();
  init_ppu();
  open_file(argv[1]);

  glutMainLoop();

  return 0;
}

void open_file(char *filename) {
  FILE *fp;
  unsigned char header[16];
  int prg_size, chr_size;
  int i, j, k = 0;

  fp = fopen(filename, "rb");
  if(fp == NULL) {
    fprintf(stderr, "%s is not opened\n", filename);
    exit(1);
  }
  
  fread(header, sizeof(unsigned char), 16, fp);
  prg_size = header[4] * 16384;
  chr_size = header[5] * 8192;
  nes_flag6 = header[6];


  for(i = 0; i < 0x8000; i += prg_size) {
    if(fseek(fp, 16, SEEK_SET) != 0) {
      fprintf(stderr, "seek error\n");
      exit(1);
    }
    for(j = 0; j < prg_size; j++, k++) {
      unsigned char data;
    
      fread(&data, sizeof(unsigned char), 1, fp);

      memory[0x8000 + k] = data;
    }
  }

  if(fseek(fp, 16 + prg_size, SEEK_SET) != 0) {
    fprintf(stderr, "seek error in second\n");
    exit(1);
  }

  for(i = 0; i < chr_size; i++) {
    unsigned char data;

    fread(&data, sizeof(unsigned char), 1, fp);

    vram[i] = data;
  }

  fclose(fp);
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glLoadIdentity();
  glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -200, 200);
}