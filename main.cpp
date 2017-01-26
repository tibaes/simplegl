#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

const int WIN_HEIGHT = 800;
const int WIN_WIDTH = 1600;

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void init(void) { glClearColor(0.0, 0.0, 0.0, 0.0); }

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Nota dez");
  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
