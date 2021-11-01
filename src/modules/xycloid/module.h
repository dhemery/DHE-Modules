#pragma once

#include "control-ids.h"
#include "params.h"

#include "components/phase-rotor.h"
#include "components/range.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/level-config.h"
#include "params/presets.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

namespace dhe {

namespace xycloid {

class Module : public rack::engine::Module {
public:
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    ThrobSpeed::config(this, Param::ThrobSpeed);
    configInput(Input::ThrobSpeedCv, "Speed CV");
    Knob::config<Attenuverter>(this, Param::ThrobSpeedAv, "Speed CV gain", 0.F);

    WobbleRatio::config(this, Param::WobbleRatio, "Ratio");
    configInput(Input::WobbleRatioCv, "Ratio CV");
    Knob::config<Attenuverter>(this, Param::WobbleRatioAv, "Ratio CV gain",
                               0.F);
    Switch::config(this, Param::WobbleRatioRange, "Direction",
                   {"In", "-In +Out", "Out"}, 2);
    Switch::config(this, Param::WobbleRatioMode, "Ratio mode",
                   {"Quantized", "Free"}, 1);

    Knob::config<Percentage>(this, Param::WobbleDepth, "Depth", 0.5F);
    configInput(Input::WobbleDepthCv, "Depth CV");
    Knob::config<Attenuverter>(this, Param::WobbleDepthAv, "Depth CV gain",
                               0.F);

    Knob::config<Angle>(this, Param::WobblePhaseOffset, "Phase", 0.F);
    configInput(Input::WobblePhaseOffsetCv, "Phase CV");
    Knob::config<Attenuverter>(this, Param::WobblePhaseOffsetAv,
                               "Phase CV gain", 0.F);

    Knob::config<Gain>(this, Param::XGain, "X gain", 1.F);
    configInput(Input::XGainCv, "X gain CV");
    config_level_range_switch(this, Param::XRange, "X range", 0);

    Knob::config<Gain>(this, Param::YGain, "Y gain", 1.F);
    configInput(Input::YGainCv, "Y gain CV");
    config_level_range_switch(this, Param::YRange, "Y range", 0);

    configOutput(Output::X, "X");
    configOutput(Output::Y, "Y");
  }

  void process(ProcessArgs const &args) override {
    auto const wobble_ratio = this->wobble_ratio();
    auto const wobble_phase_offset = wobble_ratio < 0.F
                                         ? -this->wobble_phase_offset()
                                         : this->wobble_phase_offset();

    auto const throb_speed = -this->throb_speed() * args.sampleTime;
    auto const wobble_speed = -wobble_ratio * throb_speed;
    auto const wobble_depth = this->wobble_depth();
    auto const throb_depth = 1.F - wobble_depth;

    throbber_.advance(throb_speed);
    wobbler_.advance(wobble_speed);
    auto const x = throb_depth * throbber_.cos() +
                   wobble_depth * wobbler_.cos(-wobble_phase_offset);
    auto const y = throb_depth * throbber_.sin() +
                   wobble_depth * wobbler_.sin(-wobble_phase_offset);

    outputs[Output::X].setVoltage(5.F * x_gain() * (x + x_offset()));
    outputs[Output::Y].setVoltage(5.F * y_gain() * (y + y_offset()));
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  inline auto gain(int knob_id, int cv_id) const -> float {
    return Gain::value(value_of(params[knob_id]), voltage_at(inputs[cv_id]));
  }

  auto x_gain() const -> float { return gain(Param::XGain, Input::XGainCv); }

  auto x_offset() const -> float {
    return is_pressed(params[Param::XRange]) ? 1.F : 0.F;
  }

  auto y_gain() const -> float { return gain(Param::YGain, Input::YGainCv); }

  auto y_offset() const -> float {
    return is_pressed(params[Param::YRange]) ? 1.F : 0.F;
  }

  auto throb_speed() const -> float {
    static auto constexpr rotation_per_volt = 0.1F;
    auto const knob = value_of(params[Param::ThrobSpeed]);
    auto const cv = voltage_at(inputs[Input::ThrobSpeedCv]);
    auto const attenuation = value_of(params[Param::ThrobSpeedAv]);
    auto const rotation = knob + cv * rotation_per_volt * attenuation;
    return ThrobSpeed::rotation_to_speed(rotation);
  }

  auto wobble_depth() const -> float {
    static auto constexpr wobble_depth_range = Range{0.F, 1.F};
    return wobble_depth_range.clamp(rotation(params[Param::WobbleDepth],
                                             inputs[Input::WobbleDepthCv],
                                             params[Param::WobbleDepthAv]));
  }

  // radians
  auto wobble_phase_offset() const -> float {
    auto const base = value_of(params[Param::WobblePhaseOffset]);
    auto const modifier =
        voltage_at(inputs[Input::WobblePhaseOffsetCv]) * 0.1F * 180.F;
    auto const attenuation = value_of(params[Param::WobblePhaseOffsetAv]);
    return base + modifier * attenuation;
  }

  auto wobble_ratio() -> float {
    auto *q = reinterpret_cast<WobbleRatio::Quantity *>(
        getParamQuantity(Param::WobbleRatio));
    float rotation =
        dhe::rotation(params[Param::WobbleRatio], inputs[Input::WobbleRatioCv],
                      params[Param::WobbleRatioAv]);
    return q->ratio(rotation);
  }

  PhaseRotor wobbler_{};
  PhaseRotor throbber_{};

  using Input = InputIds;
  using Output = OutputIds;
  using Param = ParamIds;
};
} // namespace xycloid
} // namespace dhe
