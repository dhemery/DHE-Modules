#pragma once

#include "./blossom-controls.h"
#include "./ratio-knob-param-quantity.h"
#include "./speed-knob-param-quantity.h"
#include "components/phase-rotor.h"
#include "components/range.h"
#include "components/sigmoid.h"
#include "config/level-config.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

#include <cmath>

namespace dhe {

namespace blossom {

static auto constexpr phase_display_range = Range{-180.F, 180.F};
static auto constexpr phase_range = Range{-0.5F, 0.5F};

class Blossom : public rack::engine::Module {
  using Controls = BlossomControls;

public:
  Blossom() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    config_speed_knob(this, Controls::SpeedKnob);
    config_attenuverter(this, Controls::SpeedAvKNob, "Speed CV gain");
    configInput(Controls::SpeedCvInput, "Speed CV");

    config_toggle<2>(this, Controls::FreeRatioSwitch, "Ratio mode",
                     {"Quantized", "Free"}, 1);

    config_ratio_knob(this, Controls::RatioKnob);
    config_attenuverter(this, Controls::RatioAvKnob, "Ratio CV gain");
    configInput(Controls::RatioCvInput, "Ratio CV");

    config_percentage_knob(this, Controls::DepthKnob, "Depth");
    config_attenuverter(this, Controls::DepthAvKnob, "Depth CV gain");
    configInput(Controls::DepthCvInput, "Depth CV");

    config_knob(this, Controls::PhaseOffsetKnob, "Phase", "°",
                phase_display_range);
    config_attenuverter(this, Controls::PhaseOffsetAvKnob, "Phase CV gain");
    configInput(Controls::PhaseCvInput, "Phase CV");

    config_gain(this, Controls::XGainKnob, "X gain");
    config_level_range_switch(this, Controls::XRangeSwitch, "X range", 0);
    configInput(Controls::XGainCvInput, "X gain CV");

    config_gain(this, Controls::YGainKnob, "Y gain");
    config_level_range_switch(this, Controls::YRangeSwitch, "Y range", 0);
    configInput(Controls::YGainCvInput, "Y gain CV");

    configOutput(Controls::XOutput, "X");
    configOutput(Controls::YOutput, "Y");
  }

  void process(ProcessArgs const &args) override {
    auto const spin_delta = -speed() * args.sampleTime;
    auto const bounce_ratio = ratio_is_free() ? ratio() : std::round(ratio());
    auto const bounce_depth = rotation_range.clamp(depth());

    spinner_.advance(spin_delta);
    bouncer_.advance(spin_delta * bounce_ratio);

    auto const angle = spinner_.angle();

    auto const radius =
        (1.F - bounce_depth) + bounce_depth * bouncer_.sin(phase_offset());
    auto const x = radius * std::cos(angle);
    auto const x_voltage = 5.F * x_gain() * (x + x_offset());
    outputs[Controls::XOutput].setVoltage(x_voltage);

    auto const y = radius * std::sin(angle);
    auto const y_voltage = 5.F * y_gain() * (y + y_offset());
    outputs[Controls::YOutput].setVoltage(y_voltage);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  inline auto ratio() const -> float {
    return ratio_range.scale(rotation(params[Controls::RatioKnob],
                                      inputs[Controls::RatioCvInput],
                                      params[Controls::RatioAvKnob]));
  }

  inline auto ratio_is_free() const -> bool {
    return position_of(params[Controls::FreeRatioSwitch]) == 1;
  }

  inline auto depth() const -> float {
    return rotation(params[Controls::DepthKnob], inputs[Controls::DepthCvInput],
                    params[Controls::DepthAvKnob]);
  }

  inline auto phase_offset() const -> float {
    return phase_range.scale(rotation(params[Controls::PhaseOffsetKnob],
                                      inputs[Controls::PhaseCvInput],
                                      params[Controls::PhaseOffsetAvKnob]));
  }

  inline auto speed() const -> float {
    return tapered_and_scaled_rotation(
        params[Controls::SpeedKnob], inputs[Controls::SpeedCvInput],
        params[Controls::SpeedAvKNob], speed_knob_taper, speed_range);
  }

  inline auto x_gain() const -> float {
    return gain_range.scale(
        rotation(params[Controls::XGainKnob], inputs[Controls::XGainCvInput]));
  }
  inline auto x_offset() const -> float {
    return selected<float, 2>(params[Controls::XRangeSwitch], {0.F, 1.F});
  };

  inline auto y_gain() const -> float {
    return gain_range.scale(
        rotation(params[Controls::YGainKnob], inputs[Controls::YGainCvInput]));
  }
  inline auto y_offset() const -> float {
    return selected<float, 2>(params[Controls::YRangeSwitch], {0.F, 1.F});
  };

  PhaseRotor spinner_{};
  PhaseRotor bouncer_{};
};
} // namespace blossom
} // namespace dhe
