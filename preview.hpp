#ifndef H_PREVIEW
#define H_PREVIEW

#include "RenderingProtocol.hpp"
#include "ctrlPts.hpp"

#include <stack>
#include <vector>

using namespace std;

namespace cg {
class Preview : public RenderingProtocol {
public:
  void display() override {}

  void update() override {}

  void onMouseClick(float x, float y) override {
    cout << "Adding control point: " << x << ", " << y << endl;
    ctrlPts.push(Point2d{x, y});
    update();
  }

  void onKeyPress(char c) override {}

private:
  stack<Point2d> ctrlPts;
};
}

#endif
