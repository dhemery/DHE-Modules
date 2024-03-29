#pragma once

namespace dhe {
namespace swave {
struct ParamId {
  enum { Curvature, Shape, CurvatureAv, Level, Count };
};

struct InputId {
  enum { CurvatureCv, Swave, Count };
};

struct OutputId {
  enum { Swave, Count };
};
} // namespace swave
}; // namespace dhe
