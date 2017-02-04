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
#include "shaders.hpp"

using namespace std;
using namespace cg;

int WIN_HEIGHT = 500;
int WIN_WIDTH = 1000;

vector<GLfloat> vertexes;
vector<Point2d> ctrlPts;
vector<Point2d> contour;
vector<Point3d> obj;

Point3d render2D(Point2d p) {
  auto x = p.x - 1.0f;
  auto y = p.y * -2.0f + 1.0f;
  return Point3d{x, y, 1.0f};
}

Point3d render3D(Point3d p) {
  auto x = p.x / 2.0f + .5f;
  auto y = p.y * -2.0f + 1.0f;
  auto z = p.z / 2.0f + .5f;
  return Point3d{x, y, z};
}

void updateModel() {
  cout << "updating" << endl;
  vertexes.clear();

  // Drawing area
  vector<GLfloat> drawingArea = {-0.95, 0.95,  1.0f, -0.05, 0.95,  1.0f,
                                 -0.05, 0.95,  1.0f, -0.05, -0.95, 1.0f,
                                 -0.05, -0.95, 1.0f, -0.95, -0.95, 1.0f,
                                 -0.95, -0.95, 1.0f, -0.95, 0.95,  1.0f};
  vertexes.insert(vertexes.end(), drawingArea.begin(), drawingArea.end());

  // Control points
  for (auto p0 : ctrlPts) {
    auto p = render2D(p0);
    vertexes.push_back(p.x);
    vertexes.push_back(p.y);
    vertexes.push_back(p.z);
  }

  // Contour
  contour = casteljau(ctrlPts);
  for (auto p0 : contour) {
    auto p = render2D(p0);
    vertexes.push_back(p.x);
    vertexes.push_back(p.y);
    vertexes.push_back(p.z);
  }

  // render triangles
  if (ctrlPts.size() > 1) {
    obj = model(contour);
    auto md_h = contour.size();
    auto md_w = obj.size() / md_h;
    auto getVTX = [&](int h, int w) { return obj[h * md_w + (w % md_w)]; };

    vector<Point3d> md_v;
    for (auto h = 0; h < md_h - 1; ++h) {
      for (auto w = 0; w < md_w; ++w) {
        // triangle 1
        md_v.push_back(getVTX(h, w));
        md_v.push_back(getVTX(h + 1, w));
        md_v.push_back(getVTX(h, w + 1));
        // triangle 2
        md_v.push_back(getVTX(h + 1, w));
        md_v.push_back(getVTX(h + 1, w + 1));
        md_v.push_back(getVTX(h, w + 1));
      }
    }

    for (auto p : md_v) {
      p = render3D(p);
      vertexes.push_back(p.x);
      vertexes.push_back(p.y);
      vertexes.push_back(p.z);
    }
  }

  glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(GLfloat),
               vertexes.data(), GL_STATIC_DRAW);
}

// Render scene
void display(GLuint &vao) {
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(vao);

  // draw drawing area
  glDrawArrays(GL_LINES, 0, 8);

  // draw controll points
  auto st_ctrlPts = 8;
  auto sz_ctrlPts = ctrlPts.size();
  glDrawArrays(GL_POINTS, st_ctrlPts, sz_ctrlPts);

  // draw contour
  auto st_contour = st_ctrlPts + sz_ctrlPts;
  auto sz_contour = contour.size();
  glDrawArrays(GL_LINE_STRIP, st_contour, sz_contour);

  // draw model
  if (ctrlPts.size() > 1) {
    auto st_model = st_contour + sz_contour;
    auto sz_model = (sz_contour - 1) * 2 * obj.size();
    glDrawArrays(GL_TRIANGLES, st_model, sz_model);
  }

  // Swap front and back buffers
  glfwSwapBuffers();
}

void initialize(GLuint &vao) {
  // Use a Vertex Array Object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Drawing area
  vector<GLfloat> drawingArea = {-0.95, 0.95,  1.0f, -0.05, 0.95,  1.0f,
                                 -0.05, 0.95,  1.0f, -0.05, -0.95, 1.0f,
                                 -0.05, -0.95, 1.0f, -0.95, -0.95, 1.0f,
                                 -0.95, -0.95, 1.0f, -0.95, 0.95,  1.0f};
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
  glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // Enable the attribute
  glEnableVertexAttribArray(position_attribute);

  // Enable points
  glEnable(GL_PROGRAM_POINT_SIZE);
}

// Called when the window is resized
void GLFWCALL window_resized(int width, int height) {
  WIN_HEIGHT = height;
  WIN_WIDTH = width;

  // Use black to clear the screen
  glClearColor(0, 0, 0, 1);

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
      ctrlPts.push_back(Point2d{fx, fy});
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
