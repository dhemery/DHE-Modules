#pragma once

#include "anchor-mode.h"
#include "anchor-source.h"
#include "completion-mode.h"
#include "components/sigmoid.h"
#include "control-ids.h"
#include "controls/common-inputs.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"
#include "interrupt-mode.h"
#include "trigger-mode.h"

#include <vector>

namespace dhe {

namespace curve_sequencer {

// Skew the progress::brightness ratio so that the "remaining" light stays
// fully lit for a little while during early progress, and the "completed"
// light reaches fully lit a little while before progress is complete.
static auto constexpr brightness_skew = 0.7F;
static auto constexpr brightness_range =
    Range{-brightness_skew, 1.F + brightness_skew};

template <typename InputT, typename OutputT, typename ParamT, typename LightT,
          int N>
class Controls {
private:
  std::vector<InputT> &inputs_;
  std::vector<OutputT> &outputs_;
  std::vector<ParamT> &params_;
  std::vector<LightT> &lights_;

  using Input = InputIds<N>;
  using Light = LightIds<N>;
  using Output = OutputIds;
  using Param = ParamIds<N>;

public:
  Controls(std::vector<InputT> &inputs, std::vector<OutputT> &outputs,
           std::vector<ParamT> &params, std::vector<LightT> &lights)
      : inputs_{inputs}, outputs_{outputs}, params_{params}, lights_{lights} {}

  // Sequence Controls

  auto gate() const -> bool {
    return is_high(inputs_[Input::Gate]) || is_pressed(params_[Param::Gate]);
  }

  auto input() const -> float { return inputs_[Input::In].getVoltage(); }

  auto is_enabled(int step) const -> bool {
    return is_pressed(params_[Param::EnableStep + step]) ||
           is_high(inputs_[Input::EnableStep + step]);
  }

  auto is_looping() const -> bool {
    return is_pressed(params_[Param::Loop]) || is_high(inputs_[Input::Loop]);
  }

  auto is_reset() const -> bool {
    return is_high(inputs_[Input::Reset]) || is_pressed(params_[Param::Reset]);
  }

  auto is_running() const -> bool {
    return is_pressed(params_[Param::Run]) || is_high(inputs_[Input::Run]);
  }

  void output(float voltage) { outputs_[Output::Out].setVoltage(voltage); }

  auto selection_start() const -> int {
    return static_cast<int>(params_[Param::SelectionStart].getValue());
  }

  auto selection_length() const -> int {
    return static_cast<int>(params_[Param::SelectionLength].getValue());
  }

  // Step controls

  auto completion_mode(int step) const -> CompletionMode {
    auto const selection =
        position_of(params_[Param::StepCompletionMode + step]);
    return static_cast<CompletionMode>(selection);
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params_[Param::StepCurvature + step]);
  }

  auto duration(int step) const -> float {
    return dhe::selectable_duration(params_[Param::StepDuration + step],
                                    params_[Param::DurationRange]);
  }

  auto interrupt_mode(int step) const -> InterruptMode {
    auto const selection =
        position_of(params_[Param::StepInterruptMode + step]);
    return static_cast<InterruptMode>(selection);
  }

  void show_inactive(int step) { set_lights(step, 0.F, 0.F); }

  void show_progress(int step, float progress) {
    auto const completed_brightness = brightness_range.scale(progress);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
  }

  auto taper(int step) const -> sigmoid::Taper const & {
    auto const selection =
        static_cast<int>(params_[Param::StepShape + step].getValue());
    return sigmoid::tapers[selection];
  }

  auto trigger_mode(int step) const -> TriggerMode {
    return static_cast<TriggerMode>(
        params_[Param::StepTriggerMode + step].getValue());
  }

private:
  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = Light::StepProgress + step + step;
    auto const remaining_light = completed_light + 1;
    lights_[completed_light].setBrightness(completed_brightness);
    lights_[remaining_light].setBrightness(remaining_brightness);
  }
};
} // namespace curve_sequencer
} // namespace dhe
