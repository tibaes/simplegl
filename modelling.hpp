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
  vector<Point3d> obj;

public:
  Modelling() {
    modelShader = InitShader("modelShader.vert", "modelShader.frag");
  }
  ~Modelling() {}

  void setCtrlPts(vector<cg::Point2d> ctrlPts) {
    obj.clear();
    if (ctrlPts.size() > 1)
      obj = cg::model(cg::casteljau(ctrlPts));
    cout << "model size: " << obj.size() << endl;
  }

  void display() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(.0, .0, .2, .0);
    glfwSwapBuffers();
  }

  void update() override {}
  void onMouseClick(float x, float y) override {}
  void onKeyPress(char c) override {}
};
}

#endif
