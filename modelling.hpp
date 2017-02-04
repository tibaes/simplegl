#ifndef H_MODELLING
#define H_MODELLING

#include "RenderingProtocol.hpp"
#include "ctrlPts.hpp"

#include <stack>
#include <vector>

namespace cg {
class Modelling : RenderingProtocol {
public:
  void display() override {}
  void update() override {}
  void onMouseClick(float x, float y) override {}
  void onKeyPress(char c) override {}

private:
};
}

#endif
