#include <iostream>
#include <array>
#include <math.h>
#include <vector>

typedef std::array<float, 2> P2D;
typedef std::array<float, 3> P3D;

struct Line2D {
  float a, b, c, sqrtAB; // a * x + b * y + c = 0
  Line2D(float a, float b, float c);
  Line2D static solve_line(const P2D &p1, const P2D &p2);
  float distance(const P2D &p) const;
};

std::vector<P2D> rdp(std::vector<P2D> points, float epsilon);
