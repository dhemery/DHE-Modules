#pragma once

namespace dhe {
namespace swave {
struct Param {
  enum { Curvature, Shape, CurvatureAv, Count };
};

struct Input {
  enum { CurvatureCv, Swave, Count };
};

struct Output {
  enum { Swave, Count };
};
} // namespace swave
}; // namespace dhe
