#ifndef H_MODELLING
#define H_MODELLING

#include "RenderingProtocol.hpp"
#include "ctrlPts.hpp"
#include "initShaders.h"

#include <memory>
#include <vector>

using namespace cg;

namespace cg {
class Modelling : public RenderingProtocol,
                  public enable_shared_from_this<Modelling> {
private:
  GLuint modelShader;
  GLuint objVBO, objVBA;
  uint trianglesObj{0};

  void createObj(vector<cg::Point2d> ctrlPts) {
    auto contour = cg::casteljau(ctrlPts);
    auto obj = cg::model(contour);

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

    vector<GLfloat> vertexes;
    for (auto p : md_v) {
      vertexes.push_back(p.x);
      vertexes.push_back(p.y);
      vertexes.push_back(p.z);
    }
    trianglesObj = vertexes.size() / 3;

    glGenVertexArrays(1, &objVBA);
    glBindVertexArray(objVBA);

    glGenBuffers(1, &objVBO);
    glBindBuffer(GL_ARRAY_BUFFER, objVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(GLfloat),
                 vertexes.data(), GL_STATIC_DRAW);

    GLint attrP = glGetAttribLocation(modelShader, "aPosition");
    glVertexAttribPointer(attrP, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrP);

    cout << "model has " << trianglesObj << " triangles" << endl;
  }

  void displayObj() {
    glBindVertexArray(objVBA);

    glm::mat4 mvp = glm::mat4(1);
    int loc = glGetUniformLocation(modelShader, "uMVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mvp));

    glDrawArrays(GL_TRIANGLES, 0, trianglesObj);
  }

public:
  void loadShaders() {
    glUseProgram(modelShader);
    cout << "loaded obj shaders" << endl;
  }
  Modelling() {
    modelShader = InitShader("modelShader.vert", "modelShader.frag");
  }
  ~Modelling() = default;

  void setCtrlPts(vector<cg::Point2d> ctrlPts) {
    trianglesObj = 0;
    if (ctrlPts.size() > 1)
      createObj(ctrlPts);
  }

  void display() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(.0, .0, .2, .0);
    if (trianglesObj > 0)
      displayObj();
    glfwSwapBuffers();
  }

  void update() override {}
  void onMouseClick(float x, float y) override {}
  void onKeyPress(char c) override {}
};
}

#endif
