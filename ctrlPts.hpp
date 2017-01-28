#ifndef CtrlPts_H
#define CtrlPts_H

#include <vector>
using namespace std;

class CtrlPts {
public:
  using Point2d = struct { int x, y; };
  CtrlPts() = default;
  ~CtrlPts() = default;

  void pushCtrlPt(Point2d p);
  void popCtrlPt();
  vector<Point2d> getRawCtrlPts() const;
  vector<Point2d> upsample(int min_points) const;

private:
  vector<Point2d> v;
};

auto ctrlPts = CtrlPts();

#endif
