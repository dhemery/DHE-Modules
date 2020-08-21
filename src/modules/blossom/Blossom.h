#pragma once

#include "BlossomControls.h"
#include "RatioKnobParamQuantity.h"
#include "SpeedKnobParamQuantity.h"
#include "components/phase-rotor.h"
#include "components/range.h"
#include "components/sigmoid.h"
#include "components/taper.h"
#include "config/level-config.h"
#include "controls/common-inputs.h"

#include <cmath>
#include <engine/Module.hpp>

namespace dhe {

namespace blossom {

class Blossom : public rack::engine::Module {
  using Controls = BlossomControls;

public:
  Blossom() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    configSpeedKnob(this, Controls::SpeedKnob);
    config_attenuverter(this, Controls::SpeedAvKNob, "Speed CV gain");

    config_toggle<2>(this, Controls::FreeRatioSwitch, "Ratio mode",
                     {"Quantized", "Free"}, 1);

    configRatioKnob(this, Controls::RatioKnob);
    config_attenuverter(this, Controls::RatioAvKnob, "Ratio CV gain");

    config_percentage_knob(this, Controls::DepthKnob, "Depth");
    config_attenuverter(this, Controls::DepthAvKnob, "Depth CV gain");

    static auto constexpr phaseDisplayRange = Range{-180.F, 180.F};
    config_knob(this, Controls::PhaseOffsetKnob, "Phase", "°",
                phaseDisplayRange);
    config_attenuverter(this, Controls::PhaseOffsetAvKnob, "Phase CV gain");

    config_gain(this, Controls::XGainKnob, "X gain");
    config_level_range_switch(this, Controls::XRangeSwitch, "X range", 0);

    config_gain(this, Controls::YGainKnob, "Y gain");
    config_level_range_switch(this, Controls::YRangeSwitch, "Y range", 0);
  }

  void process(ProcessArgs const &args) override {
    auto const spinDelta = -speed() * args.sampleTime;
    auto const bounceRatio = ratioIsFree() ? ratio() : std::round(ratio());
    auto const bounceDepth = rotation_range.clamp(depth());

    spinner.advance(spinDelta);
    bouncer.advance(spinDelta * bounceRatio);

    auto const angle = spinner.angle();

    auto const radius =
        (1.F - bounceDepth) + bounceDepth * bouncer.sin(phaseOffset());
    auto const x = radius * std::cos(angle);
    auto const xVoltage = 5.F * xGain() * (x + xOffset());
    outputs[Controls::XOutput].setVoltage(xVoltage);

    auto const y = radius * std::sin(angle);
    auto const yVoltage = 5.F * yGain() * (y + yOffset());
    outputs[Controls::YOutput].setVoltage(yVoltage);
  }

private:
  inline auto ratio() const -> float {
    return ratioRange.scale(rotation(params[Controls::RatioKnob],
                                     inputs[Controls::RatioCvInput],
                                     params[Controls::RatioAvKnob]));
  }

  inline auto ratioIsFree() const -> bool {
    return position_of(params[Controls::FreeRatioSwitch]) == 1;
  }

  inline auto depth() const -> float {
    return rotation(params[Controls::DepthKnob], inputs[Controls::DepthCvInput],
                    params[Controls::DepthAvKnob]);
  }

  inline auto phaseOffset() const -> float {
    static auto constexpr phaseRange = Range{-0.5F, 0.5F};
    return phaseRange.scale(rotation(params[Controls::PhaseOffsetKnob],
                                     inputs[Controls::PhaseCvInput],
                                     params[Controls::PhaseOffsetAvKnob]));
  }

  inline auto speed() const -> float {
    return tapered_and_scaled_rotation(
        params[Controls::SpeedKnob], inputs[Controls::SpeedCvInput],
        params[Controls::SpeedAvKNob], speedKnobTaper, speedRange);
  }

  inline auto xGain() const -> float {
    return gain_range.scale(
        rotation(params[Controls::XGainKnob], inputs[Controls::XGainCvInput]));
  }
  inline auto xOffset() const -> float {
    return selected<float, 2>(params[Controls::XRangeSwitch], {0.F, 1.F});
  };

  inline auto yGain() const -> float {
    return gain_range.scale(
        rotation(params[Controls::YGainKnob], inputs[Controls::YGainCvInput]));
  }
  inline auto yOffset() const -> float {
    return selected<float, 2>(params[Controls::YRangeSwitch], {0.F, 1.F});
  };

  PhaseRotor spinner{};
  PhaseRotor bouncer{};
};
} // namespace blossom
} // namespace dhe
