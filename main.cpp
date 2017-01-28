// Draw four triangles on a red background
#include <GL/glew.h>
#include <GL/glfw.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "ctrlPts.hpp"
#include "shaders.hpp"

using namespace std;

int WIN_HEIGHT = 500;
int WIN_WIDTH = 1000;

vector<GLfloat> vertexes;

void updateModel() { cout << "should update!" << endl; }

// Render scene
void display(GLuint &vao) {
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(vao);

  // square the drawing area
  glDrawArrays(GL_LINES, 0, 8);

  // Swap front and back buffers
  glfwSwapBuffers();
}

void initialize(GLuint &vao) {
  // Use a Vertex Array Object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Drawing area
  vector<GLfloat> drawingArea = {-0.95, 0.95,  -0.05, 0.95,  -0.05, 0.95,
                                 -0.05, -0.95, -0.05, -0.95, -0.95, -0.95,
                                 -0.95, -0.95, -0.95, 0.95};
  vertexes.insert(vertexes.end(), drawingArea.begin(), drawingArea.end());

  // Create a Vector Buffer Object that will store the vertices on video memory
  GLuint vbo;
  glGenBuffers(1, &vbo);

  // Allocate space and upload the data from CPU to GPU
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(GLfloat),
               vertexes.data(), GL_STATIC_DRAW);

  GLuint shaderProgram =
      create_program("shaders/vert.shader", "shaders/frag.shader");

  // Get the location of the attributes that enters in the vertex shader
  GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

  // Specify how the data for position can be accessed
  glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

  // Enable the attribute
  glEnableVertexAttribArray(position_attribute);

  // Enable points
  glEnable(GL_PROGRAM_POINT_SIZE);
}

// Called when the window is resized
void GLFWCALL window_resized(int width, int height) {
  WIN_HEIGHT = height;
  WIN_WIDTH = width;

  // Use red to clear the screen
  glClearColor(1, 0, 0, 1);

  // Set the viewport
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
      ctrlPts.pushCtrlPt(Point2d{fx, fy});
      updateModel();
    } else {
      cout << "mouse preview" << endl;
    }
  }
}

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW! I'm out!" << std::endl;
    exit(-1);
  }

  // Use OpenGL 3.2 core profile
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);

  // Open a window and attach an OpenGL rendering context to the window
  // surface
  if (!glfwOpenWindow(WIN_WIDTH, WIN_HEIGHT, 8, 8, 8, 0, 0, 0, GLFW_WINDOW)) {
    std::cerr << "Failed to open a window! I'm out!" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  // Register a callback function for window resize events
  glfwSetWindowSizeCallback(window_resized);

  // Register a callback function for keyboard pressed events
  glfwSetKeyCallback(keyboard);

  // Register a callback function for mouse events
  glfwSetMouseButtonCallback(mouse);

  // Print the OpenGL version
  int major, minor, rev;
  glfwGetGLVersion(&major, &minor, &rev);
  std::cout << "OpenGL - " << major << "." << minor << "." << rev << std::endl;

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW! I'm out!" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  // Create a vertex array object
  GLuint vao;

  // Initialize the data to be rendered
  initialize(vao);

  // Create a rendering loop
  int running = GL_TRUE;

  while (running) {
    // Display scene
    display(vao);

    // Pool for events
    glfwPollEvents();
    // Check if the window was closed
    running = glfwGetWindowParam(GLFW_OPENED);
  }

  // Terminate GLFW
  glfwTerminate();

  return 0;
}
