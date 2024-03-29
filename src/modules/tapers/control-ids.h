#pragma once
namespace dhe {
namespace tapers {

struct ParamId {
  enum {
    Level1,
    LevelAv1,
    LevelRange1,
    Curvature1,
    CurvatureAv1,
    Shape1,
    Level2,
    LevelAv2,
    LevelRange2,
    Curvature2,
    CurvatureAv2,
    Shape2,
    Count
  };
};

struct InputId {
  enum { LevelCv1, CurvatureCv1, LevelCv2, CurvatureCv2, Count };
};

struct OutputId {
  enum { Taper1, Taper2, Count };
};

} // namespace tapers
} // namespace dhe
