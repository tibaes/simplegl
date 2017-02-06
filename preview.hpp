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
  GLuint pointsVBO[2];
  GLuint curveVBO[2];
  int rsz_ctrlPts{0}, rsz_smooth{0};
  vector<cg::Point2d> ctrlPts;

  GLuint cubeVBO[2];

  void createCube() {

    // cube
    // ///////////////////////////////////////////////////////////////////////
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3

    // vertex coords array for glDrawArrays()
    // =====================================
    // A cube has 6 sides and each side has 2 triangles, therefore, a cube
    // consists
    // of 36 vertices (6 sides * 2 tris * 3 vertices = 36 vertices). And, each
    // vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
    // array is 108 floats (36 * 3 = 108).
    GLfloat vertices[] = {
        1,  1,  1,  -1, 1,  1,  -1, -1, 1, // v0-v1-v2 (front)
        -1, -1, 1,  1,  -1, 1,  1,  1,  1, // v2-v3-v0

        1,  1,  1,  1,  -1, 1,  1,  -1, -1, // v0-v3-v4 (right)
        1,  -1, -1, 1,  1,  -1, 1,  1,  1,  // v4-v5-v0

        1,  1,  1,  1,  1,  -1, -1, 1,  -1, // v0-v5-v6 (top)
        -1, 1,  -1, -1, 1,  1,  1,  1,  1,  // v6-v1-v0

        -1, 1,  1,  -1, 1,  -1, -1, -1, -1, // v1-v6-v7 (left)
        -1, -1, -1, -1, -1, 1,  -1, 1,  1,  // v7-v2-v1

        -1, -1, -1, 1,  -1, -1, 1,  -1, 1,  // v7-v4-v3 (bottom)
        1,  -1, 1,  -1, -1, 1,  -1, -1, -1, // v3-v2-v7

        1,  -1, -1, -1, -1, -1, -1, 1,  -1,  // v4-v7-v6 (back)
        -1, 1,  -1, 1,  1,  -1, 1,  -1, -1}; // v6-v5-v4

    // color array
    GLfloat colors[] = {1, 1, 1, 1, 1, 0, 1, 0, 0, // v0-v1-v2 (front)
                        1, 0, 0, 1, 0, 1, 1, 1, 1, // v2-v3-v0

                        1, 1, 1, 1, 0, 1, 0, 0, 1, // v0-v3-v4 (right)
                        0, 0, 1, 0, 1, 1, 1, 1, 1, // v4-v5-v0

                        1, 1, 1, 0, 1, 1, 0, 1, 0, // v0-v5-v6 (top)
                        0, 1, 0, 1, 1, 0, 1, 1, 1, // v6-v1-v0

                        1, 1, 0, 0, 1, 0, 0, 0, 0, // v1-v6-v7 (left)
                        0, 0, 0, 1, 0, 0, 1, 1, 0, // v7-v2-v1

                        0, 0, 0, 0, 0, 1, 1, 0, 1, // v7-v4-v3 (bottom)
                        1, 0, 1, 1, 0, 0, 0, 0, 0, // v3-v2-v7

                        0, 0, 1, 0, 0, 0, 0, 1, 0,  // v4-v7-v6 (back)
                        0, 1, 0, 0, 1, 1, 0, 0, 1}; // v6-v5-v4

    glGenBuffers(2, cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO[1]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
  }

  void displayCube() {
    int attrV, attrC;

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO[0]);
    attrV = glGetAttribLocation(curveShader, "aPosition");
    glVertexAttribPointer(attrV, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrV);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO[1]);
    attrC = glGetAttribLocation(curveShader, "aColor");
    glVertexAttribPointer(attrC, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrC);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisableVertexAttribArray(attrV);
    glDisableVertexAttribArray(attrC);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void createCtrlPts() {
    float color = 0.0f;
    const float z_depth = 1.0f, color_step = 1.0f / ctrlPts.size();
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

    glGenBuffers(2, pointsVBO);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vtx_points.size() * sizeof(float),
                 vtx_points.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, vtx_points_color.size() * sizeof(float),
                 vtx_points_color.data(), GL_STATIC_DRAW);
  }

  void displayCtrlPts() {
    glm::mat4 MVP = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glUseProgram(curveShader);
    int loc = glGetUniformLocation(curveShader, "uMVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));

    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO[0]);
    int attrV = glGetAttribLocation(curveShader, "aPosition");
    glVertexAttribPointer(attrV, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrV);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO[1]);
    int attrC = glGetAttribLocation(curveShader, "aColor");
    glVertexAttribPointer(attrC, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrC);

    glPointSize(3.0f);
    glDrawArrays(GL_LINE_STRIP, 0, rsz_ctrlPts);
    glDisableVertexAttribArray(attrV);
    glDisableVertexAttribArray(attrC);
  }

  void createCurve() {
    auto smooth = cg::casteljau(ctrlPts);
    float color = 0.0f;
    const float z_depth = 1.0f, color_step = 1.0f / smooth.size();

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

    glGenBuffers(2, curveVBO);

    glBindBuffer(GL_ARRAY_BUFFER, curveVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vtx_lines.size() * sizeof(float),
                 vtx_lines.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, vtx_lines_color.size() * sizeof(float),
                 vtx_lines_color.data(), GL_STATIC_DRAW);
  }

  void displayCurve() {}

public:
  bool drawCtrlPts{false}, drawCurve{false}, drawCube{true};

  Preview(GLuint _curveShader) : curveShader{_curveShader} {}
  ~Preview(){};

  void display() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 0.0, 0.0, 0.0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glfwSwapBuffers();
  }

  void update() override {}

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
