#ifndef CtrlPts_H
#define CtrlPts_H

#include <cmath>
#include <vector>
using namespace std;

namespace cg {
using Point2d = struct { float x, y; };
using Point3d = struct { float x, y, z; };

Point2d sum(Point2d a, Point2d b, float alpha) {
  auto x = alpha * a.x + (1.0f - alpha) * b.x;
  auto y = alpha * a.y + (1.0f - alpha) * b.y;
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

vector<Point3d> plane(float radius, float y, float t_step = 0.01f) {
  vector<Point3d> points;
  for (float t = 0.0f; t <= 1.0f; t += t_step) {
    auto x = radius * cos(t);
    auto z = radius * sin(t);
    points.push_back(Point3d{x, y, z});
  }
  return points;
}

vector<Point3d> model(vector<Point2d> contour) {
  vector<Point3d> points;
  for (auto pt_c : contour) {
    auto pl = plane(pt_c.x, pt_c.y);
    points.insert(points.end(), pl.begin(), pl.end());
  }
  return points;
}
}

#endif
