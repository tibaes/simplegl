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
  GLuint pointsVBO, pointsVBA;
  GLuint curveVBO, curveVBA;
  int rsz_ctrlPts{0}, rsz_smooth{0};
  vector<cg::Point2d> ctrlPts;

  void createCtrlPts() {
    rsz_ctrlPts = ctrlPts.size();

    GLfloat color = 0.0f;
    const GLfloat color_step = 1.0f / ctrlPts.size();
    vector<GLfloat> vtx_points, vtx_points_color;
    for (const auto p : ctrlPts) {
      vtx_points.push_back(GLfloat(p.x));
      vtx_points.push_back(GLfloat(-1 * p.y));
      vtx_points_color.push_back(color);
      vtx_points_color.push_back(1.0f);
      vtx_points_color.push_back(1.0f - color);
      color += color_step;
    }
    auto sz_vertexes = vtx_points.size() * sizeof(GLfloat);
    auto sz_colors = vtx_points_color.size() * sizeof(GLfloat);

    glGenVertexArrays(1, &pointsVBA);
    glBindVertexArray(pointsVBA);

    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sz_vertexes + sz_colors, NULL,
                 GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sz_vertexes, vtx_points.data());
    glBufferSubData(GL_ARRAY_BUFFER, sz_vertexes, sz_colors,
                    vtx_points_color.data());

    GLint attrP = glGetAttribLocation(curveShader, "aPosition");
    glVertexAttribPointer(attrP, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrP);

    GLint attrC = glGetAttribLocation(curveShader, "aColor");
    glVertexAttribPointer(attrC, 3, GL_FLOAT, GL_FALSE, 0,
                          (GLvoid *)sz_vertexes);
    glEnableVertexAttribArray(attrC);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(10.0f);
  }

  void displayCtrlPts() {
    glBindVertexArray(pointsVBA);
    glDrawArrays(GL_POINTS, 0, rsz_ctrlPts);
  }

  void createCurve() {
    auto smooth = cg::casteljau(ctrlPts);
    rsz_smooth = smooth.size();

    GLfloat color = 0.0f;
    const GLfloat color_step = 1.0f / smooth.size();
    vector<GLfloat> vtx_lines, vtx_lines_color;
    for (const auto p : smooth) {
      vtx_lines.push_back(GLfloat(p.x));
      vtx_lines.push_back(GLfloat(-1 * p.y));
      vtx_lines_color.push_back(color);
      vtx_lines_color.push_back(1.0f);
      vtx_lines_color.push_back(1.0f - color);
      color += color_step;
    }
    auto sz_vertexes = vtx_lines.size() * sizeof(GLfloat);
    auto sz_colors = vtx_lines_color.size() * sizeof(GLfloat);

    glGenVertexArrays(1, &curveVBA);
    glBindVertexArray(curveVBA);

    glGenBuffers(1, &curveVBO);
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO);
    glBufferData(GL_ARRAY_BUFFER, sz_vertexes + sz_colors, NULL,
                 GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sz_vertexes, vtx_lines.data());
    glBufferSubData(GL_ARRAY_BUFFER, sz_vertexes, sz_colors,
                    vtx_lines_color.data());

    GLint attrP = glGetAttribLocation(curveShader, "aPosition");
    glVertexAttribPointer(attrP, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrP);

    GLint attrC = glGetAttribLocation(curveShader, "aColor");
    glVertexAttribPointer(attrC, 3, GL_FLOAT, GL_FALSE, 0,
                          (GLvoid *)sz_vertexes);
    glEnableVertexAttribArray(attrC);
  }

  void displayCurve() {
    glBindVertexArray(curveVBA);
    glDrawArrays(GL_LINE_STRIP, 0, rsz_smooth);
  }

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

    displayLines();
    if (rsz_ctrlPts > 0)
      displayCtrlPts();
    if (rsz_ctrlPts > 1)
      displayCurve();

    glfwSwapBuffers();
  }

  void update() override {
    createCtrlPts();
    createCurve();
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
