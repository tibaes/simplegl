#ifndef CtrlPts_H
#define CtrlPts_H

#include <vector>
using namespace std;

using Point2d = struct { float x, y; };
using Point3d = struct { float x, y, z; };

class CtrlPts {
public:
  CtrlPts() = default;
  ~CtrlPts() = default;
  void pushCtrlPt(Point2d p) { v.push_back(p); }
  void popCtrlPt() { v.pop_back(); }
  vector<Point2d> getRawCtrlPts() const { return v; }

  vector<Point2d> computeCurve(int min_points) const;

private:
  vector<Point2d> v;
};

vector<Point3d> computeModel(vector<Point2d> curve);

auto ctrlPts = CtrlPts();

#endif
