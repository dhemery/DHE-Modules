#pragma once

#include "./ratio-knob-param-quantity.h"
#include "./speed-knob-param-query.h"
#include "./xycloid-controls.h"
#include "components/phase-rotor.h"
#include "components/range.h"
#include "params/common-config.h"
#include "params/level-config.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

namespace dhe {

namespace xycloid {

static auto constexpr phase_offset_range = Range{-180.F, 180.F};
static auto constexpr wobble_depth_range = Range{0.F, 1.F};

class Xycloid : public rack::engine::Module {
  using Controls = XycloidControls;

public:
  Xycloid() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    config_speed_knob(this, Controls::SpeedKnob);
    configInput(Controls::SpeedCvInput, "Speed CV");
    config_attenuverter(this, Controls::SpeedAvKnob, "Speed CV gain");

    config_ratio_knob(this, Controls::RatioKnob);
    configInput(Controls::RatioCvInput, "Ratio CV");
    config_attenuverter(this, Controls::RatioAvKnob, "Ratio CV gain");
    config_toggle<3>(this, Controls::DirectionSwitch, "Direction",
                     {"In", "-In +Out", "Out"}, 2);
    config_toggle<2>(this, Controls::FreeRatioSwitch, "Ratio mode",
                     {"Quantized", "Free"}, 1);

    config_percentage_knob(this, Controls::DepthKnob, "Depth", {0.F, 1.F});
    configInput(Controls::DepthCvInput, "Depth CV");
    config_attenuverter(this, Controls::DepthAvKnob, "Depth CV gain");

    config_knob(this, Controls::PhaseOffsetKnob, "Phase", "°",
                phase_offset_range);
    configInput(Controls::PhaseCvInput, "Phase CV");
    config_attenuverter(this, Controls::PhaseOffsetAvKnob, "Phase CV gain");

    config_gain(this, Controls::XGainKnob, "X gain");
    configInput(Controls::XGainCvInput, "X gain CV");
    config_level_range_switch(this, Controls::XRangeSwitch, "X range", 0);

    config_gain(this, Controls::YGainKnob, "Y gain");
    configInput(Controls::YGainCvInput, "Y gain CV");
    config_level_range_switch(this, Controls::YRangeSwitch, "Y range", 0);

    configOutput(Controls::XOutput, "X");
    configOutput(Controls::YOutput, "Y");
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

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
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
