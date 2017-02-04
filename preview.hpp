#ifndef H_PREVIEW
#define H_PREVIEW

#include "RenderingProtocol.hpp"
#include "ctrlPts.hpp"

#include <stack>
#include <vector>

namespace cg {
class Preview : RenderingProtocol {
public:
  void display() override {}
  void update() override {}
  void onMouseClick(float x, float y) override {}
  void onKeyPress(char c) override {}

private:
  stack<Point2d> ctrlPts;
};
}

#endif
