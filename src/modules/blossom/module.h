#pragma once

#include "control-ids.h"
#include "params.h"

#include "components/phase-rotor.h"
#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/switches.h"
#include "params/level-config.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

#include <cmath>

namespace dhe {
namespace blossom {

class Module : public rack::engine::Module {
public:
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    SpinSpeed::config(this, Param::SpinSpeed);
    Attenuverter::config(this, Param::SpinSpeedAv, "Speed CV gain");
    configInput(Input::SpinSpeedCv, "Speed CV");

    Switch::config(this, Param::BounceRatioMode, "Ratio mode",
                   {"Quantized", "Free"}, 1);

    BounceRatio::config(this, Param::BounceRatio, "Ratio");
    Attenuverter::config(this, Param::BounceRatioAv, "Ratio CV gain");
    configInput(Input::BounceRatioCv, "Ratio CV");

    Percentage::config(this, Param::BounceDepth, "Depth", 50.F);
    Attenuverter::config(this, Param::BounceDepthAv, "Depth CV gain");
    configInput(Input::BounceDepthCv, "Depth CV");

    Phase::config(this, Param::BouncePhaseOffset, "Phase");
    Attenuverter::config(this, Param::BouncePhaseOffsetAv, "Phase CV gain");
    configInput(Input::BouncePhaseOffsetCv, "Phase CV");

    Gain::config(this, Param::XGain, "X gain");
    config_level_range_switch(this, Param::XRange, "X range", 0);
    configInput(Input::XGainCv, "X gain CV");

    Gain::config(this, Param::YGain, "Y gain");
    config_level_range_switch(this, Param::YRange, "Y range", 0);
    configInput(Input::YGainCv, "Y gain CV");

    configOutput(Output::X, "X");
    configOutput(Output::Y, "Y");
  }

  void process(ProcessArgs const &args) override {
    auto const spin_delta = -spin_speed() * args.sampleTime;
    auto const bounce_ratio = bounce_ratio_is_free()
                                  ? this->bounce_ratio()
                                  : std::round(this->bounce_ratio());
    auto const bounce_depth = rotation_range.clamp(this->bounce_depth());

    spinner_.advance(spin_delta);
    bouncer_.advance(spin_delta * bounce_ratio);

    auto const angle = spinner_.angle();

    auto const radius = (1.F - bounce_depth) +
                        bounce_depth * bouncer_.sin(bounce_phase_offset());
    auto const x = radius * std::cos(angle);
    auto const x_voltage = 5.F * x_gain() * (x + x_offset());
    outputs[Output::X].setVoltage(x_voltage);

    auto const y = radius * std::sin(angle);
    auto const y_voltage = 5.F * y_gain() * (y + y_offset());
    outputs[Output::Y].setVoltage(y_voltage);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  inline auto bounce_ratio() const -> float {
    return BounceRatio::value(rotation(params[Param::BounceRatio],
                                       inputs[Input::BounceRatioCv],
                                       params[Param::BounceRatioAv]));
  }

  inline auto bounce_ratio_is_free() const -> bool {
    return position_of(params[Param::BounceRatioMode]) == 1;
  }

  inline auto bounce_depth() const -> float {
    return rotation(params[Param::BounceDepth], inputs[Input::BounceDepthCv],
                    params[Param::BounceDepthAv]);
  }

  inline auto bounce_phase_offset() const -> float {
    return Phase::value(rotation(params[Param::BouncePhaseOffset],
                                 inputs[Input::BouncePhaseOffsetCv],
                                 params[Param::BouncePhaseOffsetAv]));
  }

  inline auto spin_speed() const -> float {
    return tapered_and_scaled_rotation(
        params[Param::SpinSpeed], inputs[Input::SpinSpeedCv],
        params[Param::SpinSpeedAv], SpinSpeed::taper(), SpinSpeed::range());
  }

  inline auto x_gain() const -> float {
    return gain_range.scale(
        rotation(params[Param::XGain], inputs[Input::XGainCv]));
  }
  inline auto x_offset() const -> float {
    return selected<float, 2>(params[Param::XRange], {0.F, 1.F});
  };

  inline auto y_gain() const -> float {
    return gain_range.scale(
        rotation(params[Param::YGain], inputs[Input::YGainCv]));
  }
  inline auto y_offset() const -> float {
    return selected<float, 2>(params[Param::YRange], {0.F, 1.F});
  };

  PhaseRotor spinner_{};
  PhaseRotor bouncer_{};
};
} // namespace blossom
} // namespace dhe
