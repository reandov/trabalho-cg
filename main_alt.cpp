#include <GL/glut.h>
#include "functions.h"

void inicio();
void desenha();

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // profundidade, um framebuffer e sistema de cores RGB 255, 255, 255
  glutInitWindowPosition(100, 100); // 100 pixels para a direito e 100 pixels para baixo
  glutInitWindowSize(500, 500); // quantidade em pixels
  glutCreateWindow("CG Project");

  initialize();

  glutDisplayFunc(draw);

  glutMainLoop();

  return 0;
}