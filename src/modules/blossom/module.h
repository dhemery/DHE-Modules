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
    config(Param::Count, Input::Count, Output::Count);

    Knob::config<SpinSpeed>(this, Param::SpinSpeed, "Speed",
                            SpinSpeed::rotation(1.F));
    Knob::config<Attenuverter>(this, Param::SpinSpeedAv, "Speed CV gain");
    configInput(Input::SpinSpeedCv, "Speed CV");

    auto *ratio_knob =
        Knob::config<BounceRatio>(this, Param::BounceRatio, "Ratio");
    auto select_ratio_mode =
        [ratio_knob](BounceRatioModes::Selection selection) {
          ratio_knob->mapper().select_mode(selection);
        };
    Switch::config<BounceRatioModes>(this, Param::BounceRatioMode, "Ratio mode",
                                     BounceRatioMode::Free)
        ->on_change(select_ratio_mode);

    Knob::config<Attenuverter>(this, Param::BounceRatioAv, "Ratio CV gain");
    configInput(Input::BounceRatioCv, "Ratio CV");

    Knob::config<Percentage>(this, Param::BounceDepth, "Depth");
    Knob::config<Attenuverter>(this, Param::BounceDepthAv, "Depth CV gain");
    configInput(Input::BounceDepthCv, "Depth CV");

    Knob::config<Angle>(this, Param::BouncePhaseOffset, "Phase");
    Knob::config<Attenuverter>(this, Param::BouncePhaseOffsetAv,
                               "Phase CV gain");
    configInput(Input::BouncePhaseOffsetCv, "Phase CV");

    Knob::config<Gain>(this, Param::XGain, "X gain");
    Switch::config<VoltageRanges>(this, Param::XRange, "X select",
                                  VoltageRange::Bipolar);
    configInput(Input::XGainCv, "X gain CV");

    Knob::config<Gain>(this, Param::YGain, "Y gain");
    Switch::config<VoltageRanges>(this, Param::YRange, "Y select",
                                  VoltageRange::Bipolar);
    configInput(Input::YGainCv, "Y gain CV");

    configOutput(Output::X, "X");
    configOutput(Output::Y, "Y");
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
    outputs[Output::X].setVoltage(x_voltage);

    auto const y = radius * spinner_.sin();
    auto const y_voltage = 5.F * y_gain() * (y + y_offset());
    outputs[Output::Y].setVoltage(y_voltage);
  }

  auto bounce_ratio() const -> float {
    auto const rotation =
        rotation_of(params[Param::BounceRatio], inputs[Input::BounceRatioCv],
                    params[Param::BounceRatioAv]);
    auto const mode =
        value_of<BounceRatioModes::Selection>(params[Param::BounceRatioMode]);
    return BounceRatio::ratio(rotation, mode);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  inline auto bounce_depth() const -> float {
    auto const rotation =
        rotation_of(params[Param::BounceDepth], inputs[Input::BounceDepthCv],
                    params[Param::BounceDepthAv]);
    return Percentage::range().clamp(rotation);
  }

  // radians
  inline auto bounce_phase_offset() const -> float {
    auto const rotation = rotation_of(params[Param::BouncePhaseOffset],
                                      inputs[Input::BouncePhaseOffsetCv],
                                      params[Param::BouncePhaseOffsetAv]);
    return Angle::radians(rotation);
  }

  inline auto spin_speed() const -> float {
    auto const rotation =
        rotation_of(params[Param::SpinSpeed], inputs[Input::SpinSpeedCv],
                    params[Param::SpinSpeedAv]);
    return SpinSpeed::hertz(rotation);
  }

  inline auto gain(int knob_id, int cv_id) const -> float {
    auto const rotation = rotation_of(params[knob_id], inputs[cv_id]);
    return Gain::value(rotation);
  }

  inline auto x_gain() const -> float {
    return gain(Param::XGain, Input::XGainCv);
  }

  inline auto x_offset() const -> float {
    return value_of(params[Param::XRange]);
  };

  inline auto y_gain() const -> float {
    return gain(Param::YGain, Input::YGainCv);
  }

  inline auto y_offset() const -> float {
    return value_of(params[Param::YRange]);
  };

  PhaseRotor spinner_{};
  PhaseRotor bouncer_{};
};
} // namespace blossom
} // namespace dhe
