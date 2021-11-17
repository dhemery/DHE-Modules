#pragma once

#include "advance-mode.h"
#include "control-ids.h"
#include "generate-mode.h"

#include "signals/basic.h"
#include "signals/curvature.h"
#include "signals/durations.h"
#include "signals/voltages.h"

#include <vector>

namespace dhe {

namespace curve_sequencer {

static auto constexpr brightness_skew = 0.7F;
static auto constexpr brightness_range =
    Range{-brightness_skew, 1.F + brightness_skew};

template <typename TParam, typename TInput, typename TOutput, typename TLight,
          int N>
struct Signals {
  using ParamId = ParamIds<N>;
  using InputId = InputIds<N>;
  using LightId = LightIds<N>;

  Signals(std::vector<TParam> &params, std::vector<TInput> &inputs,
          std::vector<TOutput> &outputs, std::vector<TLight> &lights)
      : params_{params}, inputs_{inputs}, outputs_{outputs}, lights_{lights} {}

  auto advance_mode(int step) const -> AdvanceMode {
    return static_cast<AdvanceMode>(
        params_[ParamId::StepAdvanceMode + step].getValue());
  }

  auto curvature(int step) const -> float {
    return Curvature::scale(value_of(params_[ParamId::StepCurvature + step]));
  }

  auto duration(int step) const -> float {
    auto const rotation = value_of(params_[ParamId::StepDuration + step]);
    auto const range =
        value_of<DurationRangeId>(params_[ParamId::DurationRange]);
    return DurationRanges::scale(rotation, range);
  }

  auto generate_mode(int step) const -> GenerateMode {
    return static_cast<GenerateMode>(
        params_[ParamId::StepGenerateMode + step].getValue());
  }

  auto input() const -> float { return inputs_[InputId::Main].getVoltage(); }

  auto is_enabled(int step) const -> bool {
    return is_pressed(params_[ParamId::StepEnabled + step]) ||
           is_high(inputs_[InputId::StepEnabled + step]);
  }

  auto is_gated() const -> bool {
    return is_high(inputs_[InputId::Gate]) ||
           is_pressed(params_[ParamId::Gate]);
  }

  auto is_looping() const -> bool {
    return is_pressed(params_[ParamId::Loop]) ||
           is_high(inputs_[InputId::Loop]);
  }

  auto is_reset() const -> bool {
    return is_high(inputs_[InputId::Reset]) ||
           is_pressed(params_[ParamId::Reset]);
  }

  auto is_running() const -> bool {
    return is_pressed(params_[ParamId::Run]) || is_high(inputs_[InputId::Run]);
  }

  auto level(int step) const -> float {
    auto const rotation = value_of(params_[ParamId::StepLevel + step]);
    auto const range_id =
        value_of<VoltageRangeId>(params_[ParamId::LevelRange]);
    return VoltageRanges::scale(rotation, range_id);
  }

  auto output() const -> float { return outputs_[OutputId::Main].getVoltage(); }

  void output(float voltage) { outputs_[OutputId::Main].setVoltage(voltage); }

  auto selection_start() const -> int {
    return value_of<int>(params_[ParamId::SelectionStart]);
  }

  auto selection_length() const -> int {
    return static_cast<int>(value_of(params_[ParamId::SelectionLength]));
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

  auto shape(int step) const -> sigmoid::ShapeId {
    return value_of<sigmoid::ShapeId>(params_[ParamId::StepShape + step]);
  }

private:
  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = LightId::StepProgress + step + step;
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
