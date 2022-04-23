#include "rdp.h"

inline float distance(const P2D &p1, const P2D &p2) {
  const P2D vec = {p1[0] - p2[0], p1[1] - p2[1]};
  return sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
}

Line2D::Line2D(float a, float b, float c) : a(a), b(b), c(c) {
  sqrtAB = sqrt(a * a + b * b);
}

Line2D Line2D::solveLine(const P2D &p1, const P2D &p2) {
  return Line2D(p1[1] - p2[1], p2[0] - p1[0], -p2[0] * p1[1] + p1[0] * p2[1]);
}

float Line2D::distance(const P2D &p) const {
  return abs(p[0] * a + p[1] * b + c) / sqrtAB;
}

inline MaxPos findMax(std::vector<P2D> &points, Line2D line, bool samePoint,
                      P2D startPoint, uint32_t startIdx, uint32_t endIdx) {
  float maxDist = 0;
  uint32_t maxIdx = startIdx;
  for (uint32_t i = startIdx; i <= endIdx; i++) {
    float dist = 0.f;
    if (samePoint) {
      dist = distance(points[i], startPoint);
    } else {
      dist = line.distance(points[i]);
    }
    if (dist > maxDist) {
      maxDist = dist;
      maxIdx = i;
    }
  }
  return {maxDist, maxIdx};
}

void rdpRecursive(std::vector<P2D> &points, std::vector<bool> &isActive,
                  uint32_t startIdx, uint32_t endIdx, float epsilon,
                  ThreadPool &threadPool, uint32_t maxPointPerThread) {
  if (startIdx + 1 >= endIdx) {
    return;
  }
  float maxDist = 0;
  uint32_t maxIdx = startIdx;
  const Line2D line = Line2D::solveLine(points[startIdx], points[endIdx]);
  const bool samePoint = distance(points[startIdx], points[endIdx]) < 1e-8;
  if (endIdx - startIdx - 1 <= maxPointPerThread) {
    MaxPos maxPos = findMax(points, line, samePoint, points[startIdx],
                            startIdx + 1, endIdx - 1);
    if (maxPos.first > maxDist) {
      maxDist = maxPos.first;
      maxIdx = maxPos.second;
    }
  } else {
    uint32_t numThread =
        uint32_t(ceil((endIdx - startIdx - 1) * 1.0 / maxPointPerThread));
    std::vector<std::future<MaxPos>> results;
    for (uint32_t thread = 0; thread < numThread; thread++) {
      results.push_back(threadPool.enqueue(
          &findMax, std::ref(points), line, samePoint, points[startIdx],
          thread * maxPointPerThread + startIdx + 1,
          std::min<uint32_t>((thread + 1) * maxPointPerThread + startIdx + 1,
                             endIdx - 1)));
    }
    for (auto &result : results) {
      MaxPos v = result.get();
      if (v.first > maxDist) {
        maxDist = v.first;
        maxIdx = v.second;
      }
    }
  }
  if (maxDist > epsilon) {
    isActive[maxIdx] = true;
    rdpRecursive(points, isActive, startIdx, maxIdx, epsilon, threadPool,
                 maxPointPerThread);
    rdpRecursive(points, isActive, maxIdx, endIdx, epsilon, threadPool,
                 maxPointPerThread);
  }
}

std::vector<P2D> rdp(std::vector<P2D> points, float epsilon, uint8_t maxThread,
                     int32_t maxPointPerThread) {
  std::vector<P2D> result;
  if (points.size() == 0) {
    return result;
  }

  std::vector<bool> isActive;
  isActive.resize(points.size());
  std::fill(isActive.begin(), isActive.end(), false);
  isActive[0] = isActive[points.size() - 1] = true;
  ThreadPool threadPool(maxThread);
  rdpRecursive(points, isActive, 0, points.size() - 1, epsilon, threadPool,
               maxPointPerThread);

  for (uint32_t i = 0; i < points.size(); i++) {
    if (isActive[i]) {
      result.push_back(points[i]);
    }
  }
  return result;
}
