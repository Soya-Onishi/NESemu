#include"GL/glut.h"
#include"cpu.h"
#include"ppu.h"
#include"rendering.h"

#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 240

void reshape(int w, int h);

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutCreateWindow("NES EMU");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(cpu);

  init_cpu();
  init_ppu();

  glutMainLoop();

  return 0;
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glLoadIdentity();
  glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -200, 200);
}