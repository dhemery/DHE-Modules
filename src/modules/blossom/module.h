#pragma once

#include "control-ids.h"
#include "params.h"

#include "components/phase-rotor.h"
#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "signals/angular.h"
#include "signals/basic.h"
#include "signals/gain.h"
#include "signals/voltages.h"

#include "rack.hpp"

#include <cmath>

namespace dhe {
namespace blossom {

class Module : public rack::engine::Module {
public:
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    Knob::config<SpinSpeed>(this, ParamId::SpinSpeed, "Speed",
                            SpinSpeed::normalize(1.F));
    Knob::config<Attenuverter>(this, ParamId::SpinSpeedAv, "Speed CV gain");
    configInput(InputId::SpinSpeedCv, "Speed CV");

    auto *ratio_knob =
        Knob::config<BounceRatio>(this, ParamId::BounceRatio, "Ratio");
    auto select_ratio_mode = [ratio_knob](BounceRatioMode mode) {
      ratio_knob->mapper().select_mode(mode);
    };
    Switch::config<BounceRatioModes>(this, ParamId::BounceRatioMode,
                                     "Ratio mode", BounceRatioMode::Free)
        ->on_change(select_ratio_mode);

    Knob::config<Attenuverter>(this, ParamId::BounceRatioAv, "Ratio CV gain");
    configInput(InputId::BounceRatioCv, "Ratio CV");

    Knob::config<Percentage>(this, ParamId::BounceDepth, "Depth");
    Knob::config<Attenuverter>(this, ParamId::BounceDepthAv, "Depth CV gain");
    configInput(InputId::BounceDepthCv, "Depth CV");

    Knob::config<Angle>(this, ParamId::BouncePhaseOffset, "Phase");
    Knob::config<Attenuverter>(this, ParamId::BouncePhaseOffsetAv,
                               "Phase CV gain");
    configInput(InputId::BouncePhaseOffsetCv, "Phase CV");

    Knob::config<Gain>(this, ParamId::XGain, "X gain");
    Switch::config<VoltageRanges>(this, ParamId::XRange, "X range",
                                  VoltageRangeId::Bipolar);
    configInput(InputId::XGainCv, "X gain CV");

    Knob::config<Gain>(this, ParamId::YGain, "Y gain");
    Switch::config<VoltageRanges>(this, ParamId::YRange, "Y range",
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
    return Percentage::range().clamp(rotation);
  }

  // radians
  inline auto bounce_phase_offset() const -> float {
    auto const rotation = rotation_of(params[ParamId::BouncePhaseOffset],
                                      inputs[InputId::BouncePhaseOffsetCv],
                                      params[ParamId::BouncePhaseOffsetAv]);
    return Angle::radians(rotation);
  }

  inline auto spin_speed() const -> float {
    auto const rotation =
        rotation_of(params[ParamId::SpinSpeed], inputs[InputId::SpinSpeedCv],
                    params[ParamId::SpinSpeedAv]);
    return SpinSpeed::scale(rotation);
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
