#include "rdp.h"

inline float distance(const P2D &p1, const P2D &p2) {
  const P2D vec = {p1[0] - p2[0], p1[1] - p2[1]};
  return sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
}

Line2D::Line2D(float a, float b, float c) : a(a), b(b), c(c) {
  sqrtAB = sqrt(a * a + b * b);
}

Line2D Line2D::solve_line(const P2D &p1, const P2D &p2) {
  return Line2D(p1[1] - p2[1], p2[0] - p1[0], -p2[0] * p1[1] + p1[0] * p2[1]);
}

float Line2D::distance(const P2D &p) const {
  return abs(p[0] * a + p[1] * b + c) / sqrtAB;
}

void rdp_recursive(std::vector<P2D> &points, std::vector<bool> &is_active,
                   uint32_t start_idx, uint32_t end_idx, float epsilon) {
  if (start_idx + 1 >= end_idx) {
    return;
  }
  const Line2D line = Line2D::solve_line(points[start_idx], points[end_idx]);
  const bool same_point = distance(points[start_idx], points[end_idx]) < 1e-8;
  float maxDist = 0;
  float maxIdx = start_idx;
  for (uint32_t i = start_idx + 1; i < end_idx; i++) {
    if (is_active[i]) {
      float dist = 0.f;
      if (same_point) {
        dist = distance(points[i], points[start_idx]);
      } else {
        dist = line.distance(points[i]);
      }
      if (dist > maxDist) {
        maxDist = dist;
        maxIdx = i;
      }
    }
  }
  if (maxDist <= epsilon) {
    for (uint32_t i = start_idx + 1; i < end_idx; i++) {
      is_active[i] = false;
    }
  } else {
    rdp_recursive(points, is_active, start_idx, maxIdx, epsilon);
    rdp_recursive(points, is_active, maxIdx, end_idx, epsilon);
  }
}

std::vector<P2D> rdp(std::vector<P2D> points, float epsilon) {
  std::vector<P2D> result;
  if (points.size() == 0) {
    return result;
  }

  std::vector<bool> is_active;
  is_active.resize(points.size());
  std::fill(is_active.begin(), is_active.end(), true);
  rdp_recursive(points, is_active, 0, points.size() - 1, epsilon);
  
  for (uint32_t i = 0; i < points.size(); i++) {
    if (is_active[i]) {
      result.push_back(points[i]);
    }
  }
  return result;
}
