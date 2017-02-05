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
  GLuint curveShader;
  GLuint curveVBO[4];
  int rsz_ctrlPts{0}, rsz_smooth{0};
  vector<cg::Point2d> ctrlPts;

public:
  Preview(GLuint _curveShader) : curveShader{_curveShader} {}
  ~Preview(){};

  void display() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 MVP = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    glUseProgram(curveShader);
    int loc = glGetUniformLocation(curveShader, "uMVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));

    // Control points

    glBindBuffer(GL_ARRAY_BUFFER, curveVBO[0]);
    int attrV = glGetAttribLocation(curveShader, "aPosition");
    glVertexAttribPointer(attrV, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrV);

    glBindBuffer(GL_ARRAY_BUFFER, curveVBO[1]);
    int attrC = glGetAttribLocation(curveShader, "aColor");
    glVertexAttribPointer(attrC, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrC);

    cout << "rendering " << rsz_ctrlPts / 3 << " points" << endl;

    glDrawArrays(GL_LINE_STRIP, 0, rsz_ctrlPts);
    glDisableVertexAttribArray(attrV);
    glDisableVertexAttribArray(attrC);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glfwSwapBuffers();
  }

  void update() override {
    float z_depth = 0.0f, color = 1.0f, color_step = 0.0f;

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
    rsz_ctrlPts = vtx_points.size();

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
    rsz_smooth = vtx_lines.size();

    glGenBuffers(4, curveVBO);

    glBindBuffer(GL_ARRAY_BUFFER, curveVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vtx_points.size() * sizeof(float),
                 vtx_points.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, vtx_points_color.size() * sizeof(float),
                 vtx_points_color.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, curveVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, vtx_lines.size() * sizeof(float),
                 vtx_lines.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO[3]);
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
