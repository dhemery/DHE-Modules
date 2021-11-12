#pragma once
namespace dhe {
namespace cubic {
struct ParamId {
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

struct InputId {
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

struct OutputId {
  enum { Cubic, Count };
};

} // namespace cubic
} // namespace dhe
