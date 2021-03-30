#include <GL/glut.h>
#include "functions.h"

void initialize() {
  glClearColor(0,0,0,0); // limpar as cores da caixa
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  //xMin, xMax, yMin, yMax, zMin, zMax
  glOrtho(-1, 1, -1, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glBegin(GL_TRIANGLES);
  glColor3f( 1, 0, 0 ); // red
  glVertex2f( -0.8, -0.8 );
  glColor3f( 0, 1, 0 ); // green
  glVertex2f( 0.8, -0.8 );
  glColor3f( 0, 0, 1 ); // blue
  glVertex2f( 0, 0.9 );
  glEnd();

  //glFlush(); // um bufer
  glutSwapBuffers(); //alterna entre buffers
}