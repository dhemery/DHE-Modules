#pragma once

#include "control-ids.h"
#include "params.h"

#include "components/phase-rotor.h"
#include "components/range.h"
#include "components/sigmoid.h"
#include "params/float-params.h"
#include "params/level-config.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

#include <cmath>

namespace dhe {

namespace blossom {

class Module : public rack::engine::Module {
public:
  Module() {
    config(Params::N, Inputs::N, Outputs::N);

    SpinSpeed::config(this, Params::SpinSpeed);
    Attenuverter::config(this, Params::SpinSpeedAv, "Speed CV gain");
    configInput(Inputs::SpinSpeedCv, "Speed CV");

    config_toggle<2>(this, Params::BounceRatioMode, "Ratio mode",
                     {"Quantized", "Free"}, 1);

    BounceRatio::config(this, Params::BounceRatio, "Ratio");
    Attenuverter::config(this, Params::BounceRatioAv, "Ratio CV gain");
    configInput(Inputs::BounceRatioCv, "Ratio CV");

    Percentage::config(this, Params::Depth, "Depth");
    Attenuverter::config(this, Params::DepthAv, "Depth CV gain");
    configInput(Inputs::DepthCv, "Depth CV");

    Phase::config(this, Params::Phase, "Phase");
    Attenuverter::config(this, Params::PhaseAv, "Phase CV gain");
    configInput(Inputs::PhaseCv, "Phase CV");

    Gain::config(this, Params::XGain, "X gain");
    config_level_range_switch(this, Params::XRange, "X range", 0);
    configInput(Inputs::XGainCv, "X gain CV");

    Gain::config(this, Params::YGain, "Y gain");
    config_level_range_switch(this, Params::YRange, "Y range", 0);
    configInput(Inputs::YGainCv, "Y gain CV");

    configOutput(Outputs::XOutput, "X");
    configOutput(Outputs::YOutput, "Y");
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
    outputs[Outputs::XOutput].setVoltage(x_voltage);

    auto const y = radius * std::sin(angle);
    auto const y_voltage = 5.F * y_gain() * (y + y_offset());
    outputs[Outputs::YOutput].setVoltage(y_voltage);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  inline auto ratio() const -> float {
    return BounceRatio::value(rotation(params[Params::BounceRatio],
                                       inputs[Inputs::BounceRatioCv],
                                       params[Params::BounceRatioAv]));
  }

  inline auto ratio_is_free() const -> bool {
    return position_of(params[Params::BounceRatioMode]) == 1;
  }

  inline auto depth() const -> float {
    return rotation(params[Params::Depth], inputs[Inputs::DepthCv],
                    params[Params::DepthAv]);
  }

  inline auto phase_offset() const -> float {
    return Phase::value(rotation(params[Params::Phase], inputs[Inputs::PhaseCv],
                                 params[Params::PhaseAv]));
  }

  inline auto speed() const -> float {
    return tapered_and_scaled_rotation(
        params[Params::SpinSpeed], inputs[Inputs::SpinSpeedCv],
        params[Params::SpinSpeedAv], SpinSpeed::taper, SpinSpeed::range);
  }

  inline auto x_gain() const -> float {
    return gain_range.scale(
        rotation(params[Params::XGain], inputs[Inputs::XGainCv]));
  }
  inline auto x_offset() const -> float {
    return selected<float, 2>(params[Params::XRange], {0.F, 1.F});
  };

  inline auto y_gain() const -> float {
    return gain_range.scale(
        rotation(params[Params::YGain], inputs[Inputs::YGainCv]));
  }
  inline auto y_offset() const -> float {
    return selected<float, 2>(params[Params::YRange], {0.F, 1.F});
  };

  PhaseRotor spinner_{};
  PhaseRotor bouncer_{};
};
} // namespace blossom
} // namespace dhe
