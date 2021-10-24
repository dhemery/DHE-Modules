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

template <typename TParam, typename TInput, typename TOutput, typename TLight,
          int N>
struct Signals {
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Output = OutputIds;
  using Light = LightIds<N>;

  Signals(std::vector<TParam> &params, std::vector<TInput> &inputs,
          std::vector<TOutput> &outputs, std::vector<TLight> &lights)
      : params_{params}, inputs_{inputs}, outputs_{outputs}, lights_{lights} {}

  auto advance_mode(int step) const -> AdvanceMode {
    return static_cast<AdvanceMode>(
        params_[Param::AdvanceMode + step].getValue());
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params_[Param::Curvature + step]);
  }

  auto duration(int step) const -> float {
    return dhe::selectable_duration(params_[Param::Duration + step],
                                    params_[Param::DurationRange]);
  }

  auto generate_mode(int step) const -> GenerateMode {
    return static_cast<GenerateMode>(
        params_[Param::GenerateMode + step].getValue());
  }

  auto input() const -> float {
    return inputs_[Input::CurveSequencerInput].getVoltage();
  }

  auto is_enabled(int step) const -> bool {
    return is_pressed(params_[Param::Enabled + step]) ||
           is_high(inputs_[Input::EnabledInputs + step]);
  }

  auto is_gated() const -> bool {
    return is_high(inputs_[Input::GateInput]) ||
           is_pressed(params_[Param::Gate]);
  }

  auto is_looping() const -> bool {
    return is_pressed(params_[Param::Loop]) ||
           is_high(inputs_[Input::LoopInput]);
  }

  auto is_reset() const -> bool {
    return is_high(inputs_[Input::ResetInput]) ||
           is_pressed(params_[Param::Reset]);
  }

  auto is_running() const -> bool {
    return is_pressed(params_[Param::Run]) || is_high(inputs_[Input::RunInput]);
  }

  auto level(int step) const -> float {
    return dhe::selectable_level(params_[Param::Level + step],
                                 params_[Param::LevelRange]);
  }

  auto output() const -> float {
    return outputs_[Output::CurveSequencerOutput].getVoltage();
  }

  void output(float voltage) {
    outputs_[Output::CurveSequencerOutput].setVoltage(voltage);
  }

  auto selection_start() const -> int {
    return static_cast<int>(params_[Param::SelectionStart].getValue());
  }

  auto selection_length() const -> int {
    return static_cast<int>(params_[Param::SelectionLength].getValue());
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
        static_cast<int>(params_[Param::Shape + step].getValue());
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

  std::vector<TParam> &params_;
  std::vector<TInput> &inputs_;
  std::vector<TOutput> &outputs_;
  std::vector<TLight> &lights_;
};
} // namespace curve_sequencer

} // namespace dhe
