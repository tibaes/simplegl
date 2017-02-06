#ifndef H_PREVIEW
#define H_PREVIEW

#include "RenderingProtocol.hpp"
#include "ctrlPts.hpp"
#include "initShaders.h"

#include <memory>
#include <vector>

using namespace std;

namespace cg {
class Preview : public RenderingProtocol,
                public enable_shared_from_this<Preview> {
private:
  GLuint curveShader;
  GLuint linesVBO, linesVBA;
  GLuint pointsVBO[2];
  GLuint curveVBO[2];
  int rsz_ctrlPts{0}, rsz_smooth{0};
  vector<cg::Point2d> ctrlPts;

  void createCtrlPts() {
    float color = 0.0f;
    const float color_step = 1.0f / ctrlPts.size();
    vector<float> vtx_points, vtx_points_color;
    for (const auto p : ctrlPts) {
      vtx_points.push_back(p.x);
      vtx_points.push_back(p.y);
      vtx_points_color.push_back(color);
      vtx_points_color.push_back(1.0f);
      vtx_points_color.push_back(1.0f - color);
      vtx_points_color.push_back(1.0f);
      color += color_step;
    }
    rsz_ctrlPts = vtx_points.size();

    glGenBuffers(2, pointsVBO);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vtx_points.size() * sizeof(float),
                 vtx_points.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, vtx_points_color.size() * sizeof(float),
                 vtx_points_color.data(), GL_STATIC_DRAW);
  }

  void displayCtrlPts() {
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO[0]);
    int attrV = glGetAttribLocation(curveShader, "aPosition");
    glVertexAttribPointer(attrV, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrV);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO[1]);
    int attrC = glGetAttribLocation(curveShader, "aColor");
    glVertexAttribPointer(attrC, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrC);

    glPointSize(3.0f);
    glDrawArrays(GL_POINTS, 0, rsz_ctrlPts);
    glDisableVertexAttribArray(attrV);
    glDisableVertexAttribArray(attrC);

    // glEnable(GL_PROGRAM_POINT_SIZE);
  }

  void createCurve() {
    auto smooth = cg::casteljau(ctrlPts);
    float color = 0.0f;
    const float color_step = 1.0f / smooth.size();

    vector<float> vtx_lines, vtx_lines_color;
    for (const auto p : smooth) {
      vtx_lines.push_back(p.x);
      vtx_lines.push_back(p.y);
      vtx_lines_color.push_back(color);
      vtx_lines_color.push_back(1.0f);
      vtx_lines_color.push_back(1.0f - color);
      vtx_lines_color.push_back(1.0f);
      color += color_step;
    }
    rsz_smooth = vtx_lines.size();

    glGenBuffers(2, curveVBO);

    glBindBuffer(GL_ARRAY_BUFFER, curveVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vtx_lines.size() * sizeof(float),
                 vtx_lines.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, vtx_lines_color.size() * sizeof(float),
                 vtx_lines_color.data(), GL_STATIC_DRAW);
  }

  void displayCurve() {}

  void createLines() {
    glGenVertexArrays(1, &linesVBA);
    glBindVertexArray(linesVBA);

    vector<GLfloat> vertexes = {-0.95, -0.95, -0.95, 0.95,  0.95,
                                0.95,  0.95,  -0.95, -0.95, -0.95};
    vector<GLfloat> colors(5 * 3, 1.0);
    auto sz_vertexes = vertexes.size() * sizeof(GLfloat);
    auto sz_colors = colors.size() * sizeof(GLfloat);

    glGenBuffers(1, &linesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
    glBufferData(GL_ARRAY_BUFFER, sz_vertexes + sz_colors, NULL,
                 GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sz_vertexes, vertexes.data());
    glBufferSubData(GL_ARRAY_BUFFER, sz_vertexes, sz_colors, colors.data());

    GLint attrP = glGetAttribLocation(curveShader, "aPosition");
    glVertexAttribPointer(attrP, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrP);

    GLint attrC = glGetAttribLocation(curveShader, "aColor");
    glVertexAttribPointer(attrC, 3, GL_FLOAT, GL_FALSE, 0,
                          (GLvoid *)sz_vertexes);
    glEnableVertexAttribArray(attrC);
  }

  void displayLines() {
    glBindVertexArray(linesVBA);
    glDrawArrays(GL_LINE_STRIP, 0, 5);
  }

public:
  Preview() {
    curveShader = InitShader("curveShader.vert", "curveShader.frag");
    createLines();
  }
  ~Preview(){};

  void display() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 0.0, 0.0, 0.0);

    // glm::mat4 MVP = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    // glUseProgram(curveShader);
    // int loc = glGetUniformLocation(curveShader, "uMVP");
    // glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));
    // displayCtrlPts();
    displayLines();

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    glfwSwapBuffers();
  }

  void update() override {
    // createCtrlPts();
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
