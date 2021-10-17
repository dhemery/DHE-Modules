#pragma once

#include "control-ids.h"
#include "params.h"

#include "components/phase-rotor.h"
#include "components/range.h"
#include "params/common-config.h"
#include "params/float-params.h"
#include "params/level-config.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

namespace dhe {

namespace xycloid {

class Module : public rack::engine::Module {
public:
  Module() {
    config(Param::ParameterCount, Input::InputCount, Output::OutputCount);

    ThrobSpeed::config(this, Param::SpeedKnob);
    configInput(Input::SpeedCvInput, "Speed CV");
    Attenuverter::config(this, Param::SpeedAvKnob, "Speed CV gain");

    WobbleRatio::config(this, Param::RatioKnob);
    configInput(Input::RatioCvInput, "Ratio CV");
    Attenuverter::config(this, Param::RatioAvKnob, "Ratio CV gain");
    config_toggle<3>(this, Param::DirectionSwitch, "Direction",
                     {"In", "-In +Out", "Out"}, 2);
    config_toggle<2>(this, Param::FreeRatioSwitch, "Ratio mode",
                     {"Quantized", "Free"}, 1);

    Percentage::config(this, Param::DepthKnob, "Depth");
    configInput(Input::DepthCvInput, "Depth CV");
    Attenuverter::config(this, Param::DepthAvKnob, "Depth CV gain");

    Phase::config(this, Param::PhaseOffsetKnob, "Phase");
    configInput(Input::PhaseCvInput, "Phase CV");
    Attenuverter::config(this, Param::PhaseOffsetAvKnob, "Phase CV gain");

    Gain::config(this, Param::XGainKnob, "X gain");
    configInput(Input::XGainCvInput, "X gain CV");
    config_level_range_switch(this, Param::XRangeSwitch, "X range", 0);

    Gain::config(this, Param::YGainKnob, "Y gain");
    configInput(Input::YGainCvInput, "Y gain CV");
    config_level_range_switch(this, Param::YRangeSwitch, "Y range", 0);

    configOutput(Output::XOutput, "X");
    configOutput(Output::YOutput, "Y");
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

    outputs[Output::XOutput].setVoltage(5.F * x_gain() * (x + x_offset()));
    outputs[Output::YOutput].setVoltage(5.F * y_gain() * (y + y_offset()));
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto x_gain() const -> float {
    return gain_range.scale(
        rotation(params[Param::XGainKnob], inputs[Input::XGainCvInput]));
  }

  auto x_offset() const -> float {
    return is_pressed(params[Param::XRangeSwitch]) ? 1.F : 0.F;
  }

  auto y_gain() const -> float {
    return gain_range.scale(
        rotation(params[Param::YGainKnob], inputs[Input::YGainCvInput]));
  }

  auto y_offset() const -> float {
    return is_pressed(params[Param::YRangeSwitch]) ? 1.F : 0.F;
  }

  auto depth() const -> float {
    static auto constexpr wobble_depth_range = Range{0.F, 1.F};
    return wobble_depth_range.clamp(rotation(params[Param::DepthKnob],
                                             inputs[Input::DepthCvInput],
                                             params[Param::DepthAvKnob]));
  }

  auto phase() const -> float {
    return Phase::value(rotation(params[Param::PhaseOffsetKnob],
                                 inputs[Input::PhaseCvInput],
                                 params[Param::PhaseOffsetAvKnob]));
  }

  auto ratio() const -> float {
    return WobbleRatio::ratio(this, rotation(params[Param::RatioKnob],
                                             inputs[Input::RatioCvInput],
                                             params[Param::RatioAvKnob]));
  }

  auto speed() const -> float {
    return tapered_and_scaled_rotation(
        params[Param::SpeedKnob], inputs[Input::SpeedCvInput],
        params[Param::SpeedAvKnob], ThrobSpeed::taper, ThrobSpeed::range);
  }
  PhaseRotor wobbler_{};
  PhaseRotor throbber_{};

  using Input = InputIds;
  using Output = OutputIds;
  using Param = ParamIds;
};
} // namespace xycloid
} // namespace dhe
