#ifndef H_PREVIEW
#define H_PREVIEW

#include "RenderingProtocol.hpp"
#include "ctrlPts.hpp"

#include <memory>
#include <stack>
#include <vector>

using namespace std;

namespace cg {
class Preview : public RenderingProtocol,
                public enable_shared_from_this<Preview> {
private:
  GLuint axisShader;
  GLuint axisVBO[3];
  stack<Point2d> ctrlPts;

public:
  Preview(GLuint _axisShader) : axisShader{_axisShader} {}
  ~Preview(){};

  void display() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 MVP = glm::ortho(-1.0, 1.0, -1.0, 1.0);

    glUseProgram(axisShader);
    int loc = glGetUniformLocation(axisShader, "uMVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));

    glfwSwapBuffers();
  }

  void update() override {}

  void onMouseClick(float x, float y) override {
    cout << "Adding control point: " << x << ", " << y << endl;
    ctrlPts.push(Point2d{x, y});
    update();
  }

  void onKeyPress(char c) override {
    cout << "Removing the last control point." << endl;
    if (c == 'R' || c == 'r') {
      ctrlPts.pop();
      update();
    }
  }
};
}

#endif
