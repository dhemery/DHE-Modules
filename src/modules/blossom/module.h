#pragma once

#include "control-ids.h"
#include "params.h"

#include "components/phase-rotor.h"
#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "signals/angular.h"
#include "signals/common-inputs.h"
#include "signals/gain.h"
#include "signals/voltage-ranges.h"

#include "rack.hpp"

#include <cmath>

namespace dhe {
namespace blossom {

class Module : public rack::engine::Module {
public:
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    SpinSpeed::config(this, Param::SpinSpeed);
    Knob::config<Attenuverter>(this, Param::SpinSpeedAv, "Speed CV gain", 0.5F);
    configInput(Input::SpinSpeedCv, "Speed CV");

    Switch::config(this, Param::BounceRatioMode, "Ratio mode",
                   {"Quantized", "Free"}, 1);

    BounceRatio::config(this, Param::BounceRatio, "Ratio");
    Knob::config<Attenuverter>(this, Param::BounceRatioAv, "Ratio CV gain",
                               0.5F);
    configInput(Input::BounceRatioCv, "Ratio CV");

    Knob::config<Percentage>(this, Param::BounceDepth, "Depth", 0.5F);
    Knob::config<Attenuverter>(this, Param::BounceDepthAv, "Depth CV gain",
                               0.5F);
    configInput(Input::BounceDepthCv, "Depth CV");

    Knob::config<Angle>(this, Param::BouncePhaseOffset, "Phase", 0.5F);
    Knob::config<Attenuverter>(this, Param::BouncePhaseOffsetAv,
                               "Phase CV gain", 0.5F);
    configInput(Input::BouncePhaseOffsetCv, "Phase CV");

    Knob::config<Gain>(this, Param::XGain, "X gain", 0.5F);
    Picker::config<VoltageRanges>(this, Param::XRange, "X range",
                                  VoltageRanges::Unipolar);
    configInput(Input::XGainCv, "X gain CV");

    Knob::config<Gain>(this, Param::YGain, "Y gain", 0.5F);
    Picker::config<VoltageRanges>(this, Param::YRange, "Y range",
                                  VoltageRanges::Bipolar);
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

    auto const radius = (1.F - bounce_depth) +
                        bounce_depth * bouncer_.sin(bounce_phase_offset());
    auto const x = radius * spinner_.cos();
    auto const x_voltage = 5.F * x_gain() * (x + x_offset());
    outputs[Output::X].setVoltage(x_voltage);

    auto const y = radius * spinner_.sin();
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

  // radians
  inline auto bounce_phase_offset() const -> float {
    return Angle::radians(rotation(params[Param::BouncePhaseOffset],
                                   inputs[Input::BouncePhaseOffsetCv],
                                   params[Param::BouncePhaseOffsetAv]));
  }

  inline auto spin_speed() const -> float {
    auto const rotation =
        dhe::rotation(params[Param::SpinSpeed], inputs[Input::SpinSpeedCv],
                      params[Param::SpinSpeedAv]);
    return SpinSpeed::range().scale(SpinSpeed::taper().apply(rotation));
  }

  inline auto gain(int knob_id, int cv_id) const -> float {
    return Gain::value(rotation(params[knob_id], inputs[cv_id]));
  }

  inline auto x_gain() const -> float {
    return gain(Param::XGain, Input::XGainCv);
  }

  inline auto x_offset() const -> float {
    return selected<float, 2>(params[Param::XRange], {0.F, 1.F});
  };

  inline auto y_gain() const -> float {
    return gain(Param::YGain, Input::YGainCv);
  }

  inline auto y_offset() const -> float {
    return selected<float, 2>(params[Param::YRange], {0.F, 1.F});
  };

  PhaseRotor spinner_{};
  PhaseRotor bouncer_{};
};
} // namespace blossom
} // namespace dhe
