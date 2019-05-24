#pragma once

#include "Module.h"
#include "modules/components/Rotor.h"

namespace dhe {

class Blossom : public Module {
public:
  Blossom();
  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    SpinKnob,
    SpinAvKNob,
    BounceKnob,
    BounceAvKnob,
    BounceLockSwitch,
    DepthKnob,
    DepthAvKnob,
    PhaseKnob,
    PhaseAvKnob,
    XGainKnob,
    YGainKnob,
    XRangeSwitch,
    YRangeSwitch,
    ParameterCount
  };
  enum InputIds { SpinCvInput, BounceCvInput, DepthCvInput, PhaseCvInput, XGainCvInput, YGainCvInput, InputCount };
  enum OutputIds { XOutput, YOutput, OutputCount };

private:
  auto bounce() -> float;
  auto depth() -> float;
  auto isBounceFree() -> bool;
  auto offset(int param) -> float;
  auto phase() -> float;
  auto spin(float sampleTime) -> float;
  auto xGain() -> float;
  auto xOffset() -> float;
  auto yGain() -> float;
  auto yOffset() -> float;

  static auto constexpr speedCurvature = 0.8f;
  Rotor spinner{};
  Rotor bouncer{};
};
} // namespace dhe
