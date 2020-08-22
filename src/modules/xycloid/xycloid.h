#pragma once

#include "components/phase-rotor.h"
#include "components/range.h"
#include "config/common-config.h"
#include "config/level-config.h"
#include "controls/common-inputs.h"
#include "ratio-knob-param-quantity.h"
#include "speed-knob-param-query.h"
#include "xycloid-controls.h"

#include <engine/Module.hpp>

namespace dhe {

namespace xycloid {

class Xycloid : public rack::engine::Module {
  using Controls = XycloidControls;

public:
  Xycloid() {
    static auto constexpr phase_offset_range = Range{-180.F, 180.F};
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    config_speed_knob(this, Controls::SpeedKnob);
    config_attenuverter(this, Controls::SpeedAvKnob, "Speed CV gain");

    config_ratio_knob(this, Controls::RatioKnob);
    config_attenuverter(this, Controls::RatioAvKnob, "Ratio CV gain");
    config_toggle<3>(this, Controls::DirectionSwitch, "Direction",
                     {"In", "-In +Out", "Out"}, 2);
    config_toggle<2>(this, Controls::FreeRatioSwitch, "Ratio mode",
                     {"Quantized", "Free"}, 1);

    config_percentage_knob(this, Controls::DepthKnob, "Depth", {0.F, 1.F});
    config_attenuverter(this, Controls::DepthAvKnob, "Depth CV gain");

    config_knob(this, Controls::PhaseOffsetKnob, "Phase", "°",
                phase_offset_range);
    config_attenuverter(this, Controls::PhaseOffsetAvKnob, "Phase CV gain");

    config_gain(this, Controls::XGainKnob, "X gain");
    config_level_range_switch(this, Controls::XRangeSwitch, "X range", 0);

    config_gain(this, Controls::YGainKnob, "Y gain");
    config_level_range_switch(this, Controls::YRangeSwitch, "Y range", 0);
  }

  void process(ProcessArgs const &args) override {
    auto const wobble_ratio = ratio();
    auto const wobble_phase_offset = wobble_ratio < 0.F ? -phase() : phase();

    auto const throb_speed = -speed() * args.sampleTime;
    auto const wobble_speed = -wobble_ratio * throb_speed;
    auto const wobble_depth = depth();
    auto const throb_depth = 1.F - wobble_depth;

    throbber_.advance(throb_speed);
    wobbler_.advance(wobble_speed);
    auto const x = throb_depth * throbber_.cos() +
                   wobble_depth * wobbler_.cos(-wobble_phase_offset);
    auto const y = throb_depth * throbber_.sin() +
                   wobble_depth * wobbler_.sin(-wobble_phase_offset);

    outputs[Controls::XOutput].setVoltage(5.F * x_gain() * (x + x_offset()));
    outputs[Controls::YOutput].setVoltage(5.F * y_gain() * (y + y_offset()));
  }

private:
  auto x_gain() const -> float {
    return gain_range.scale(
        rotation(params[Controls::XGainKnob], inputs[Controls::XGainCvInput]));
  }

  auto x_offset() const -> float {
    return is_pressed(params[Controls::XRangeSwitch]) ? 1.F : 0.F;
  }

  auto y_gain() const -> float {
    return gain_range.scale(
        rotation(params[Controls::YGainKnob], inputs[Controls::YGainCvInput]));
  }

  auto y_offset() const -> float {
    return is_pressed(params[Controls::YRangeSwitch]) ? 1.F : 0.F;
  }

  auto depth() const -> float {
    static auto constexpr wobble_depth_range = Range{0.F, 1.F};
    return wobble_depth_range.clamp(rotation(params[Controls::DepthKnob],
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
    return tapered_and_scaled_rotation(
        params[Controls::SpeedKnob], inputs[Controls::SpeedCvInput],
        params[Controls::SpeedAvKnob], speed_knob_taper, speed_range);
  }
  PhaseRotor wobbler_{};
  PhaseRotor throbber_{};
};
} // namespace xycloid
} // namespace dhe
