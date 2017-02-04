#ifndef H_RenderingProtocol
#define H_RenderingProtocol

namespace cg {
class RenderingProtocol {
public:
  virtual ~RenderingProtocol();
  virtual void display() = 0;
  virtual void update() = 0;
  virtual void onMouseClick(float x, float y);
  virtual void onKeyPress(char c);
};
}

#endif
