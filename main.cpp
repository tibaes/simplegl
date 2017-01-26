#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>
using namespace std;

const int WIN_HEIGHT = 800;
const int WIN_WIDTH = 1600;

vector<pair<int, int>> ctrlPts;

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void mouse(int button, int state, int x, int y) {
  if ((button == GLUT_RIGHT_BUTTON || button == GLUT_LEFT_BUTTON) &&
      state == GLUT_DOWN) {
    if (x < WIN_WIDTH / 2) {
      cout << "adding controll point: (" << x << ", " << y << ")" << endl;
      ctrlPts.push_back(pair<int, int>(x, y));
    } else {
      cout << "outside drawing area: (" << x << ", " << y << ")" << endl;
    }
  }
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'q':
    cout << "reset" << endl;
    break;
  default:
    cout << "another key.." << endl;
    break;
  }
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
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
