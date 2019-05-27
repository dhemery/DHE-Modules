#pragma once

#include "Module.h"
#include "modules/components/Rotor.h"

namespace dhe {

class Blossom : public Module {
public:
  Blossom();
  void process(const ProcessArgs &args) override;
  auto isBounceFree() -> bool;

  enum ParameterIds {
    SpinKnob,
    SpinAvKNob,
    BounceRatioKnob,
    BounceRatioAvKnob,
    BounceRatioModeSwitch,
    BounceDepthKnob,
    BounceDepthAvKnob,
    BouncePhaseOffsetKnob,
    BouncePhaseOffsetAvKnob,
    XGainKnob,
    YGainKnob,
    XRangeSwitch,
    YRangeSwitch,
    ParameterCount
  };
  enum InputIds {
    SpinCvInput,
    BounceRatioCvInput,
    BounceDepthCvInput,
    BouncePhaseCvInput,
    XGainCvInput,
    YGainCvInput,
    InputCount
  };
  enum OutputIds { XOutput, YOutput, OutputCount };

private:
  auto phase() -> float;
  auto xGain() -> float;
  auto xOffset() -> float;
  auto yGain() -> float;
  auto yOffset() -> float;

  std::function<float()> bounce;
  std::function<float()> depth;
  std::function<float()> spin;
  Rotor spinner{};
  Rotor bouncer{};
};
} // namespace dhe
