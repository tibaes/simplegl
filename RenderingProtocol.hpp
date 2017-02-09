#ifndef H_RenderingProtocol
#define H_RenderingProtocol

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
#include <stack>
#include <vector>

namespace cg {
class RenderingProtocol {
public:
  virtual ~RenderingProtocol() {}
  virtual void display() {}
  virtual void update() {}
  virtual void onMouseClick(float x, float y) {}
  virtual void onKeyPress(char c) {}
};
}

#endif
