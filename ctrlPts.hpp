#ifndef CtrlPts_H
#define CtrlPts_H

#include <vector>
using namespace std;

namespace cg {
using Point2d = struct { float x, y; };
using Point3d = struct { float x, y, z; };

Point2d sum(Point2d a, Point2d b, float alpha) {
  auto x = (a.x + b.x) * alpha;
  auto y = (a.y + b.y) * alpha;
  return Point2d{x, y};
}

Point2d casteljauKernel(vector<Point2d> control, float alpha) {
  if (control.size() == 1)
    return control.front();
  vector<Point2d> new_control;
  for (auto ip = 1; ip < control.size(); ++ip)
    new_control.push_back(sum(control[ip - 1], control[ip], alpha));
  return casteljauKernel(new_control, alpha);
}

vector<Point2d> casteljau(vector<Point2d> control, float alpha_step = 0.01f) {
  vector<Point2d> points;
  for (float alpha = 0.0f; alpha <= 1.0f; alpha += alpha_step)
    points.push_back(casteljauKernel(control, alpha));
  return points;
}
}

#endif
