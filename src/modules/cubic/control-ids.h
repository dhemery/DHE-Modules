#pragma once
namespace dhe {
namespace cubic {
struct Param {
  enum {
    ACoefficient,
    BCoefficient,
    CCoefficient,
    DCoefficient,
    InputGain,
    OutputGain,
    Count
  };
};

struct Input {
  enum {
    Cubic,
    ACoefficientCv,
    BCoefficientCv,
    CCoefficientCv,
    DCoefficientCv,
    InputGainCv,
    OutputGainCv,
    Count
  };
};

struct Output {
  enum { Cubic, Count };
};

} // namespace cubic
} // namespace dhe
