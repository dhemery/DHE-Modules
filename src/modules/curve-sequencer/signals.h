#pragma once

#include "advance-mode.h"
#include "control-ids.h"
#include "generate-mode.h"

#include "signals/common-inputs.h"
#include "signals/curvature-inputs.h"
#include "signals/duration-inputs.h"
#include "signals/level-inputs.h"

#include <vector>

namespace dhe {

namespace curve_sequencer {

static auto constexpr brightness_skew = 0.7F;
static auto constexpr brightness_range =
    Range{-brightness_skew, 1.F + brightness_skew};

template <typename TInput, typename TOutput, typename TParam, typename TLight,
          int N>
class Signals {
private:
  TInput &inputs_;
  TOutput &outputs_;
  TParam &params_;
  TLight &lights_;

public:
  Signals(TInput &inputs, TOutput &outputs, TParam &params, TLight &lights)
      : inputs_{inputs}, outputs_{outputs}, params_{params}, lights_{lights} {}

  auto condition(int step) const -> AdvanceMode {
    return static_cast<AdvanceMode>(
        params_[Param::ConditionSwitches + step].getValue());
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params_[Param::CurveKnobs + step]);
  }

  auto duration(int step) const -> float {
    return dhe::selectable_duration(params_[Param::DurationKnobs + step],
                                    params_[Param::DurationRangeSwitch]);
  }

  auto input() const -> float {
    return inputs_[Input::CurveSequencerInput].getVoltage();
  }

  auto is_enabled(int step) const -> bool {
    return is_pressed(params_[Param::EnabledButtons + step]) ||
           is_high(inputs_[Input::EnabledInputs + step]);
  }

  auto is_gated() const -> bool {
    return is_high(inputs_[Input::GateInput]) ||
           is_pressed(params_[Param::GateButton]);
  }

  auto is_looping() const -> bool {
    return is_pressed(params_[Param::LoopButton]) ||
           is_high(inputs_[Input::LoopInput]);
  }

  auto is_reset() const -> bool {
    return is_high(inputs_[Input::ResetInput]) ||
           is_pressed(params_[Param::ResetButton]);
  }

  auto is_running() const -> bool {
    return is_pressed(params_[Param::RunButton]) ||
           is_high(inputs_[Input::RunInput]);
  }

  auto level(int step) const -> float {
    return dhe::selectable_level(params_[Param::LevelKnobs + step],
                                 params_[Param::LevelRangeSwitch]);
  }

  auto mode(int step) const -> GenerateMode {
    return static_cast<GenerateMode>(
        params_[Param::ModeSwitches + step].getValue());
  }

  auto output() const -> float {
    return outputs_[Output::CurveSequencerOutput].getVoltage();
  }

  void output(float voltage) {
    outputs_[Output::CurveSequencerOutput].setVoltage(voltage);
  }

  auto selection_start() const -> int {
    return static_cast<int>(params_[Param::SelectionStartKnob].getValue());
  }

  auto selection_length() const -> int {
    return static_cast<int>(params_[Param::SelectionLengthKnob].getValue());
  }

  void show_inactive(int step) { set_lights(step, 0.F, 0.F); }

  void show_progress(int step, float progress) {
    // Skew the progress::brightness ratio so that the "remaining" light stays
    // fully lit for a little while during early progress, and the "completed"
    // light reaches fully lit a little while before progress is complete.
    auto const completed_brightness = brightness_range.scale(progress);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
  }

  auto taper(int step) const -> sigmoid::Taper const & {
    auto const selection =
        static_cast<int>(params_[Param::ShapeSwitches + step].getValue());
    return sigmoid::tapers[selection];
  }

private:
  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = Light::ProgressLights + step + step;
    auto const remaining_light = completed_light + 1;
    lights_[completed_light].setBrightness(completed_brightness);
    lights_[remaining_light].setBrightness(remaining_brightness);
  }

  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Param = ParamIds<N>;
  using Output = OutputIds;
};
} // namespace curve_sequencer

} // namespace dhe
