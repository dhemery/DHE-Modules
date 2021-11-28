#pragma once

#include "bounce-ratio.h"
#include "control-ids.h"
#include "controls.h"
#include "spin-speed.h"

#include "components/phase-rotor.h"
#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "controls/voltage-controls.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "signals/linear-signals.h"
#include "signals/voltage-signals.h"

#include "rack.hpp"

#include <cmath>

namespace dhe {
namespace blossom {

class Module : public rack::engine::Module {
public:
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    SpinSpeedKnob::config(this, ParamId::SpinSpeed, "Speed");
    AttenuverterKnob::config(this, ParamId::SpinSpeedAv, "Speed CV gain");
    configInput(InputId::SpinSpeedCv, "Speed CV");

    auto *ratio_knob =
        BounceRatioKnob::config(this, ParamId::BounceRatio, "Ratio");
    BounceRatioModeSwitch::config(this, ParamId::BounceRatioMode, "Ratio mode")
        ->add_knob(ratio_knob);

    AttenuverterKnob::config(this, ParamId::BounceRatioAv, "Ratio CV gain");
    configInput(InputId::BounceRatioCv, "Ratio CV");

    PercentageKnob::config(this, ParamId::BounceDepth, "Depth", 50.F);
    AttenuverterKnob::config(this, ParamId::BounceDepthAv, "Depth CV gain");
    configInput(InputId::BounceDepthCv, "Depth CV");

    PhaseKnob::config(this, ParamId::BouncePhaseOffset, "Phase");
    AttenuverterKnob::config(this, ParamId::BouncePhaseOffsetAv,
                             "Phase CV gain");
    configInput(InputId::BouncePhaseOffsetCv, "Phase CV");

    GainKnob::config(this, ParamId::XGain, "X gain");
    VoltageRangeSwitch::config(this, ParamId::XRange, "X range",
                               VoltageRangeId::Bipolar);
    configInput(InputId::XGainCv, "X gain CV");

    GainKnob::config(this, ParamId::YGain, "Y gain");
    VoltageRangeSwitch::config(this, ParamId::YRange, "Y range",
                               VoltageRangeId::Bipolar);
    configInput(InputId::YGainCv, "Y gain CV");

    configOutput(OutputId::X, "X");
    configOutput(OutputId::Y, "Y");
  }

  void process(ProcessArgs const &args) override {
    auto const spin_delta = -spin_speed() * args.sampleTime;
    auto const bounce_depth = this->bounce_depth();

    spinner_.advance(spin_delta);
    bouncer_.advance(spin_delta * bounce_ratio());

    auto const radius = (1.F - bounce_depth) +
                        bounce_depth * bouncer_.sin(bounce_phase_offset());
    auto const x = radius * spinner_.cos();
    auto const x_voltage = 5.F * x_gain() * (x + x_offset());
    outputs[OutputId::X].setVoltage(x_voltage);

    auto const y = radius * spinner_.sin();
    auto const y_voltage = 5.F * y_gain() * (y + y_offset());
    outputs[OutputId::Y].setVoltage(y_voltage);
  }

  auto bounce_ratio() const -> float {
    auto const rotation = rotation_of(params[ParamId::BounceRatio],
                                      inputs[InputId::BounceRatioCv],
                                      params[ParamId::BounceRatioAv]);
    auto const mode =
        value_of<BounceRatioMode>(params[ParamId::BounceRatioMode]);
    return BounceRatio::scale(rotation, mode);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  inline auto bounce_depth() const -> float {
    auto const rotation = rotation_of(params[ParamId::BounceDepth],
                                      inputs[InputId::BounceDepthCv],
                                      params[ParamId::BounceDepthAv]);
    return Rotation::range.clamp(rotation);
  }

  // radians
  inline auto bounce_phase_offset() const -> float {
    auto const rotation = rotation_of(params[ParamId::BouncePhaseOffset],
                                      inputs[InputId::BouncePhaseOffsetCv],
                                      params[ParamId::BouncePhaseOffsetAv]);
    return Phase::scale(rotation);
  }

  inline auto spin_speed() -> float {
    auto const rotation = rotation_of(params[ParamId::SpinSpeed]);
    auto const modulation = modulation_of(inputs[InputId::SpinSpeedCv],
                                          params[ParamId::SpinSpeedAv]);
    return SpinSpeed::scale(rotation, modulation);
  }

  inline auto gain(int knob_id, int cv_id) const -> float {
    auto const rotation = rotation_of(params[knob_id], inputs[cv_id]);
    return Gain::scale(rotation);
  }

  inline auto x_gain() const -> float {
    return gain(ParamId::XGain, InputId::XGainCv);
  }

  inline auto x_offset() const -> float {
    return value_of(params[ParamId::XRange]);
  };

  inline auto y_gain() const -> float {
    return gain(ParamId::YGain, InputId::YGainCv);
  }

  inline auto y_offset() const -> float {
    return value_of(params[ParamId::YRange]);
  };

  PhaseRotor spinner_{};
  PhaseRotor bouncer_{};
};
} // namespace blossom
} // namespace dhe
