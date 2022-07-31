#pragma once

namespace RDP {
struct Point2d {
  float x_, y_;
  Point2d() : x_(0.0), y_(0.0) {}
  Point2d(float x, float y) : x_(x), y_(y) {}
};
}  // namespace RDP
