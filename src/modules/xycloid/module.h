#pragma once

#include "control-ids.h"
#include "controls.h"
#include "throb-speed.h"
#include "wobble-ratio.h"

#include "components/phase-rotor.h"
#include "components/range.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "controls/voltage-controls.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "signals/linear-signals.h"
#include "signals/voltage-signals.h"

#include "rack.hpp"

namespace dhe {

namespace xycloid {

class Module : public rack::engine::Module {
public:
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    ThrobSpeedKnob::config(this, ParamId::ThrobSpeed, "Speed");
    configInput(InputId::ThrobSpeedCv, "Speed CV");
    AttenuverterKnob::config(this, ParamId::ThrobSpeedAv, "Speed CV gain");

    auto *ratio_knob =
        WobbleRatioKnob::config(this, ParamId::WobbleRatio, "Ratio");

    configInput(InputId::WobbleRatioCv, "Ratio CV");
    AttenuverterKnob::config(this, ParamId::WobbleRatioAv, "Ratio CV gain");
    WobbleRatioRangeSwitch::config(this, ParamId::WobbleRatioRange, "Direction")
        ->add_knob(ratio_knob);
    WobbleRatioModeSwitch::config(this, ParamId::WobbleRatioMode, "Ratio mode")
        ->add_knob(ratio_knob);

    PercentageKnob::config(this, ParamId::WobbleDepth, "Depth", 50.F);
    configInput(InputId::WobbleDepthCv, "Depth CV");
    AttenuverterKnob::config(this, ParamId::WobbleDepthAv, "Depth CV gain");

    PhaseKnob::config(this, ParamId::WobblePhaseOffset, "Phase");
    configInput(InputId::WobblePhaseOffsetCv, "Phase CV");
    AttenuverterKnob::config(this, ParamId::WobblePhaseOffsetAv,
                             "Phase CV gain");

    GainKnob::config(this, ParamId::XGain, "X gain");
    configInput(InputId::XGainCv, "X gain CV");
    VoltageRangeSwitch::config(this, ParamId::XRange, "X range",
                               VoltageRangeId::Bipolar);

    GainKnob::config(this, ParamId::YGain, "Y gain");
    configInput(InputId::YGainCv, "Y gain CV");
    VoltageRangeSwitch::config(this, ParamId::YRange, "Y range",
                               VoltageRangeId::Bipolar);

    configOutput(OutputId::X, "X");
    configOutput(OutputId::Y, "Y");
  }

  void process(ProcessArgs const &args) override {
    auto const wobble_ratio = this->wobble_ratio();
    auto const wobble_phase_offset = wobble_ratio < 0.F
                                         ? this->wobble_phase_offset()
                                         : -this->wobble_phase_offset();

    auto const throb_speed = -this->throb_speed() * args.sampleTime;
    auto const wobble_speed = -wobble_ratio * throb_speed;
    auto const wobble_depth = this->wobble_depth();
    auto const throb_depth = 1.F - wobble_depth;

    throbber_.advance(throb_speed);
    wobbler_.advance(wobble_speed);
    auto const x = throb_depth * throbber_.cos() +
                   wobble_depth * wobbler_.cos(wobble_phase_offset);
    auto const y = throb_depth * throbber_.sin() +
                   wobble_depth * wobbler_.sin(wobble_phase_offset);

    outputs[OutputId::X].setVoltage(5.F * x_gain() * (x + x_offset()));
    outputs[OutputId::Y].setVoltage(5.F * y_gain() * (y + y_offset()));
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  inline auto gain(int knob_id, int cv_id) const -> float {
    auto const rotation = rotation_of(params[knob_id], inputs[cv_id]);
    return Gain::scale(rotation);
  }

  auto x_gain() const -> float {
    return gain(ParamId::XGain, InputId::XGainCv);
  }

  auto x_offset() const -> float {
    return is_pressed(params[ParamId::XRange]) ? 1.F : 0.F;
  }

  auto y_gain() const -> float {
    return gain(ParamId::YGain, InputId::YGainCv);
  }

  auto y_offset() const -> float {
    return is_pressed(params[ParamId::YRange]) ? 1.F : 0.F;
  }

  auto throb_speed() const -> float {
    return ThrobSpeed::scale(rotation_of(params[ParamId::ThrobSpeed]),
                             modulation_of(inputs[InputId::ThrobSpeedCv],
                                           params[ParamId::ThrobSpeedAv]));
  }

  auto wobble_depth() const -> float {
    static auto constexpr range = Range{0.F, 1.F};
    auto const rotation = rotation_of(params[ParamId::WobbleDepth],
                                      inputs[InputId::WobbleDepthCv],
                                      params[ParamId::WobbleDepthAv]);
    return range.clamp(rotation);
  }

  // radians
  auto wobble_phase_offset() const -> float {
    auto const rotation = rotation_of(params[ParamId::WobblePhaseOffset],
                                      inputs[InputId::WobblePhaseOffsetCv],
                                      params[ParamId::WobblePhaseOffsetAv]);
    return Phase::scale(rotation);
  }

  auto wobble_ratio() -> float {
    auto const rotation = rotation_of(params[ParamId::WobbleRatio],
                                      inputs[InputId::WobbleRatioCv],
                                      params[ParamId::WobbleRatioAv]);
    auto const range =
        value_of<WobbleRatioRangeId>(params[ParamId::WobbleRatioRange]);
    auto const mode =
        value_of<WobbleRatioMode>(params[ParamId::WobbleRatioMode]);
    return WobbleRatio::scale(rotation, range, mode);
  }

  PhaseRotor wobbler_{};
  PhaseRotor throbber_{};
};
} // namespace xycloid
} // namespace dhe
