#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "ctrlPts.hpp"
#include "initShaders.h"

using namespace std;
using namespace cg;

int WIN_HEIGHT = 1000;
int WIN_WIDTH = 1000;
GLuint axisShader;
GLuint axisVBO[3];

vector<Point2d> ctrlPts;

void update() {}

void display() {}

// Initializing OpenGL and GLEW stuff
void initGL(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glPointSize(3.0);
  if (glewInit()) {
    cout << "Unable to initialize GLEW ... exiting" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
  cout << "Opengl Version: " << glGetString(GL_VERSION) << endl;
  cout << "Opengl Vendor : " << glGetString(GL_VENDOR) << endl;
  cout << "Opengl Render : " << glGetString(GL_RENDERER) << endl;
  cout << "Opengl Shading Language Version : "
       << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

// Called when the window is resized
void GLFWCALL window_resized(int width, int height) {
  WIN_HEIGHT = height;
  WIN_WIDTH = width;
  glClearColor(0, 0, 0, 0);
  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers();
}

// Called for keyboard events
void keyboard(int key, int action) {
  if (key == 'Q' && action == GLFW_PRESS) {
    glfwTerminate();
    exit(0);
  }
}

// Called for mouse button
void mouse(int button, int action) {
  if (action == 1) {
    int x, y;
    glfwGetMousePos(&x, &y);
    if (x < WIN_WIDTH / 2) {
      float fx = x / float(WIN_WIDTH / 2);
      float fy = y / float(WIN_HEIGHT);
      cout << "mouse drawable: " << fx << ", " << fy << endl;
      ctrlPts.push_back(Point2d{fx, fy});
      update();
    } else {
      cout << "mouse preview" << endl;
    }
  }
}

int main() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW! I'm out!" << std::endl;
    exit(-1);
  }
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
  if (!glfwOpenWindow(WIN_WIDTH, WIN_HEIGHT, 8, 8, 8, 0, 0, 0, GLFW_WINDOW)) {
    std::cerr << "Failed to open a window! I'm out!" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  glfwSetWindowSizeCallback(window_resized);
  glfwSetKeyCallback(keyboard);
  glfwSetMouseButtonCallback(mouse);

  initGL();
  axisShader = InitShader("axisShader.vert", "axisShader.frag");

  int running = GL_TRUE;
  while (running) {
    display();
    glfwPollEvents();
    running = glfwGetWindowParam(GLFW_OPENED);
  }

  glfwTerminate();
  return 0;
}
