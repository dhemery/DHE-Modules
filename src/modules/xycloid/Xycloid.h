#pragma once

#include "RatioKnobParamQuantity.h"
#include "SpeedKnobParamQuantity.h"
#include "XycloidControls.h"
#include "components/PhaseRotor.h"
#include "components/Range.h"
#include "components/Taper.h"
#include "config/CommonConfig.h"
#include "config/LevelConfig.h"
#include "controls/CommonInputs.h"

#include <config/CommonConfig.h>
#include <engine/Module.hpp>

namespace dhe {

namespace xycloid {

class Xycloid : public rack::engine::Module {
  using Controls = XycloidControls;

public:
  Xycloid() {
    static auto constexpr phaseOffsetRange = Range{-180.F, 180.F};
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    configSpeedKnob(this, Controls::SpeedKnob);
    config_attenuverter(this, Controls::SpeedAvKnob, "Speed CV gain");

    configRatioKnob(this, Controls::RatioKnob);
    config_attenuverter(this, Controls::RatioAvKnob, "Ratio CV gain");
    config_toggle<3>(this, Controls::DirectionSwitch, "Direction",
                     {"In", "-In +Out", "Out"}, 2);
    config_toggle<2>(this, Controls::FreeRatioSwitch, "Ratio mode",
                     {"Quantized", "Free"}, 1);

    config_percentage_knob(this, Controls::DepthKnob, "Depth", {0.F, 1.F});
    config_attenuverter(this, Controls::DepthAvKnob, "Depth CV gain");

    config_knob(this, Controls::PhaseOffsetKnob, "Phase", "°",
                phaseOffsetRange);
    config_attenuverter(this, Controls::PhaseOffsetAvKnob, "Phase CV gain");

    config_gain(this, Controls::XGainKnob, "X gain");
    configLevelRangeSwitch(this, Controls::XRangeSwitch, "X range", 0);

    config_gain(this, Controls::YGainKnob, "Y gain");
    configLevelRangeSwitch(this, Controls::YRangeSwitch, "Y range", 0);
  }

  void process(ProcessArgs const &args) override {
    auto const wobbleRatio = ratio();
    auto const wobblePhaseOffset = wobbleRatio < 0.F ? -phase() : phase();

    auto const throbSpeed = -speed() * args.sampleTime;
    auto const wobbleSpeed = -wobbleRatio * throbSpeed;
    auto const wobbleDepth = depth();
    auto const throbDepth = 1.F - wobbleDepth;

    throbber.advance(throbSpeed);
    wobbler.advance(wobbleSpeed);
    auto const x = throbDepth * throbber.cos() +
                   wobbleDepth * wobbler.cos(-wobblePhaseOffset);
    auto const y = throbDepth * throbber.sin() +
                   wobbleDepth * wobbler.sin(-wobblePhaseOffset);

    outputs[Controls::XOutput].setVoltage(5.F * xGain() * (x + xOffset()));
    outputs[Controls::YOutput].setVoltage(5.F * yGain() * (y + yOffset()));
  }

private:
  auto xGain() const -> float {
    return gainRange.scale(
        rotation(params[Controls::XGainKnob], inputs[Controls::XGainCvInput]));
  }

  auto xOffset() const -> float {
    return isPressed(params[Controls::XRangeSwitch]) ? 1.F : 0.F;
  }

  auto yGain() const -> float {
    return gainRange.scale(
        rotation(params[Controls::YGainKnob], inputs[Controls::YGainCvInput]));
  }

  auto yOffset() const -> float {
    return isPressed(params[Controls::YRangeSwitch]) ? 1.F : 0.F;
  }

  auto depth() const -> float {
    static auto constexpr wobbleDepthRange = Range{0.F, 1.F};
    return wobbleDepthRange.clamp(rotation(params[Controls::DepthKnob],
                                           inputs[Controls::DepthCvInput],
                                           params[Controls::DepthAvKnob]));
  }

  auto phase() const -> float {
    return rotation(params[Controls::PhaseOffsetKnob],
                    inputs[Controls::PhaseCvInput],
                    params[Controls::PhaseOffsetAvKnob]) -
           0.5F;
  }

  auto ratio() const -> float {
    return xycloid::ratio(this, rotation(params[Controls::RatioKnob],
                                         inputs[Controls::RatioCvInput],
                                         params[Controls::RatioAvKnob]));
  }

  auto speed() const -> float {
    return taperedAndScaledRotation(
        params[Controls::SpeedKnob], inputs[Controls::SpeedCvInput],
        params[Controls::SpeedAvKnob], speedKnobTaper, speedRange);
  }
  PhaseRotor wobbler{};
  PhaseRotor throbber{};
};
} // namespace xycloid
} // namespace dhe
