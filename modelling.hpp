#ifndef H_MODELLING
#define H_MODELLING

#include "RenderingProtocol.hpp"
#include "ctrlPts.hpp"

#include <memory>
#include <stack>
#include <vector>

namespace cg {
class Modelling : public RenderingProtocol,
                  public enable_shared_from_this<Modelling> {
public:
  void display() override {}
  void update() override {}
  void onMouseClick(float x, float y) override {}
  void onKeyPress(char c) override {}

private:
};
}

#endif
