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
  GLuint linesVBO[1], linesVBA[1];
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
    // Use a Vertex Array Object
    glGenVertexArrays(1, linesVBA);
    glBindVertexArray(linesVBA[0]);

    // Drawing area
    vector<GLfloat> drawingArea = {-0.95, 0.95,  -0.05, 0.95,  -0.05, 0.95,
                                   -0.05, -0.95, -0.05, -0.95, -0.95, -0.95,
                                   -0.95, -0.95, -0.95, 0.95};

    // Create a Vector Buffer Object that will store the vertices on video
    // memory
    glGenBuffers(1, linesVBO);

    // Allocate space and upload the data from CPU to GPU
    glBindBuffer(GL_ARRAY_BUFFER, linesVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, drawingArea.size() * sizeof(GLfloat),
                 drawingArea.data(), GL_STATIC_DRAW);

    // Get the location of the attributes that enters in the vertex shader
    GLint position_attribute = glGetAttribLocation(curveShader, "aPosition");

    // Specify how the data for position can be accessed
    glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Enable the attribute
    glEnableVertexAttribArray(position_attribute);

    // Enable points
    glEnable(GL_PROGRAM_POINT_SIZE);
  }

  void displayLines() {
    glBindVertexArray(linesVBA[0]);
    glDrawArrays(GL_LINES, 0, 8);
  }

public:
  Preview() {
    cout << "starting shaders... " << endl;
    curveShader = InitShader("curveShader.vert", "curveShader.frag");
    cout << "curve shaders loaded!" << endl;
    createLines();
    cout << "lines created.. " << endl;
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
