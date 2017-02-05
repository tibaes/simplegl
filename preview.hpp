#ifndef H_PREVIEW
#define H_PREVIEW

#include "RenderingProtocol.hpp"
#include "ctrlPts.hpp"

#include <memory>
#include <vector>

using namespace std;

namespace cg {
class Preview : public RenderingProtocol,
                public enable_shared_from_this<Preview> {
private:
  GLuint axisShader;
  GLuint axisVBO[4];
  vector<cg::Point2d> ctrlPts;

public:
  Preview(GLuint _axisShader) : axisShader{_axisShader} {}
  ~Preview(){};

  void display() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 MVP = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glUseProgram(axisShader);
    int loc = glGetUniformLocation(axisShader, "uMVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));

    glfwSwapBuffers();
  }

  void update() override {
    float z_depth = 1.0f, color = 1.0f, color_step = 0.0f;

    color = 0.0f;
    color_step = 1.0f / ctrlPts.size();
    vector<float> vtx_points, vtx_points_color;
    for (const auto p : ctrlPts) {
      vtx_points.push_back(p.x);
      vtx_points.push_back(p.y);
      vtx_points.push_back(z_depth);
      vtx_points_color.push_back(color);
      vtx_points_color.push_back(1.0f);
      vtx_points_color.push_back(1.0f - color);
      vtx_points_color.push_back(1.0f);
      color += color_step;
    }

    auto smooth = cg::casteljau(ctrlPts);
    color = 0.0f;
    color_step = 1.0f / smooth.size();
    vector<float> vtx_lines, vtx_lines_color;
    for (const auto p : smooth) {
      vtx_lines.push_back(p.x);
      vtx_lines.push_back(p.y);
      vtx_lines.push_back(z_depth);
      vtx_lines_color.push_back(color);
      vtx_lines_color.push_back(1.0f);
      vtx_lines_color.push_back(1.0f - color);
      vtx_lines_color.push_back(1.0f);
      color += color_step;
    }

    glGenBuffers(4, axisVBO);

    glBindBuffer(GL_ARRAY_BUFFER, axisVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vtx_points.size() * sizeof(float),
                 vtx_points.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, vtx_points_color.size() * sizeof(float),
                 vtx_points_color.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, axisVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, vtx_lines.size() * sizeof(float),
                 vtx_lines.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO[3]);
    glBufferData(GL_ARRAY_BUFFER, vtx_lines_color.size() * sizeof(float),
                 vtx_lines_color.data(), GL_STATIC_DRAW);
  }

  void onMouseClick(float x, float y) override {
    cout << "Adding control point: " << x << ", " << y << endl;
    ctrlPts.push_back(cg::Point2d{x, y});
    update();
  }

  void onKeyPress(char c) override {
    if (c == 'R') {
      cout << "Removed the last control point." << endl;
      ctrlPts.pop_back();
      update();
    }
    if (c == 'C') {
      cout << "Cleared all control points." << endl;
      ctrlPts.clear();
    }
  }
};
}

#endif
