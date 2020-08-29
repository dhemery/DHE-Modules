#pragma once

#include "components/sigmoid.h"
#include "controls/common-inputs.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"
#include "source.h"
#include "triggers.h"

#include <vector>

namespace dhe {

namespace curve_sequencer_2 {

// Skew the progress::brightness ratio so that the "remaining" light stays
// fully lit for a little while during early progress, and the "completed"
// light reaches fully lit a little while before progress is complete.
static auto constexpr brightness_skew = 0.7F;
static auto constexpr brightness_range =
    Range{-brightness_skew, 1.F + brightness_skew};

template <typename PortT, typename ParamT, typename LightT, int N>
class Controls {
private:
  std::vector<PortT> &inputs_;
  std::vector<PortT> &outputs_;
  std::vector<ParamT> &params_;
  std::vector<LightT> &lights_;

public:
  Controls(std::vector<PortT> &inputs, std::vector<PortT> &outputs,
           std::vector<ParamT> &params, std::vector<LightT> &lights)
      : inputs_{inputs}, outputs_{outputs}, params_{params}, lights_{lights} {}

  // Sequence Controls

  auto input() const -> float {
    return inputs_[enum_index(Input::CurveSequencer)].getVoltage();
  }

  auto is_enabled(int step) const -> bool {
    return is_pressed(params_[enum_index(Param::EnableStep, step)]) ||
           is_high(inputs_[enum_index(Input::EnableStep, step)]);
  }

  auto is_gated() const -> bool {
    return is_high(inputs_[enum_index(Input::Gate)]) ||
           is_pressed(params_[enum_index(Param::Gate)]);
  }

  auto is_looping() const -> bool {
    return is_pressed(params_[enum_index(Param::Loop)]) ||
           is_high(inputs_[enum_index(Input::Loop)]);
  }

  auto is_reset() const -> bool {
    return is_high(inputs_[enum_index(Input::Reset)]) ||
           is_pressed(params_[enum_index(Param::Reset)]);
  }

  auto is_running() const -> bool {
    return is_pressed(params_[enum_index(Param::Run)]) ||
           is_high(inputs_[enum_index(Input::Run)]);
  }

  auto output() const -> float {
    return outputs_[enum_index(Output::CurveSequencer)].getVoltage();
  }

  void output(float voltage) {
    outputs_[enum_index(Output::CurveSequencer)].setVoltage(voltage);
  }

  auto selection_start() const -> int {
    return static_cast<int>(
        params_[enum_index(Param::SelectionStart)].getValue());
  }

  auto selection_length() const -> int {
    return static_cast<int>(
        params_[enum_index(Param::SelectionLength)].getValue());
  }

  // Step controls

  auto advance_on_end_of_curve(int step) const -> bool {
    return position_of(
               params_[enum_index(Param::StepAdvancesOnEndOfCurve, step)]) == 1;
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params_[enum_index(Param::StepCurvature, step)]);
  }

  auto duration(int step) const -> float {
    return dhe::selectable_duration(
        params_[enum_index(Param::StepDuration, step)],
        params_[enum_index(Param::DurationRange)]);
  }

  auto end_level(int step) const -> float {
    return dhe::selectable_level(params_[enum_index(Param::StepEndLevel, step)],
                                 params_[enum_index(Param::LevelRange)]);
  }

  auto end_source(int step) const -> Source {
    return static_cast<Source>(
        params_[enum_index(Param::StepEndSource, step)].getValue());
  }

  auto interrupt_on_trigger(int step) const -> bool {
    return position_of(
               params_[enum_index(Param::StepInterruptsOnTrigger, step)]) == 1;
  }

  void show_inactive(int step) { set_lights(step, 0.F, 0.F); }

  void show_progress(int step, float progress) {
    auto const completed_brightness = brightness_range.scale(progress);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
  }

  auto start_level(int step) const -> float {
    return dhe::selectable_level(
        params_[enum_index(Param::StepStartLevel, step)],
        params_[enum_index(Param::LevelRange)]);
  }

  auto start_source(int step) const -> Source {
    return static_cast<Source>(
        params_[enum_index(Param::StepStartSource, step)].getValue());
  }

  auto taper(int step) const -> sigmoid::Taper const & {
    auto const selection = static_cast<int>(
        params_[enum_index(Param::StepShape, step)].getValue());
    return sigmoid::tapers[selection];
  }

  auto track_end_source(int step) const -> bool {
    return position_of(params_[enum_index(Param::StepTracksEndSource, step)]) ==
           1;
  }

  auto track_start_source(int step) const -> bool {
    return position_of(
               params_[enum_index(Param::StepTracksStartSource, step)]) == 1;
  }

  auto trigger_mode(int step) const -> TriggerMode {
    return static_cast<TriggerMode>(
        params_[enum_index(Param::StepTriggerMode, step)].getValue());
  }

  enum class Param {
    Run,
    Gate,
    SelectionStart,
    SelectionLength,
    Loop,
    Reset,
    DurationRange,
    LevelRange,
    // Above: Overall module/sequence params
    // Below: Step params
    StepCurvature,
    StepDuration = StepCurvature + N,
    EnableStep = StepDuration + N,
    StepEndLevel = EnableStep + N,
    StepTriggerMode = StepEndLevel + N,
    StepInterruptsOnTrigger = StepTriggerMode + N,
    StepShape = StepInterruptsOnTrigger + N,
    // The rest are new in 1.3.0
    StepStartLevel = StepShape + N,
    StepAdvancesOnEndOfCurve = StepStartLevel + N,
    StepStartSource = StepAdvancesOnEndOfCurve + N,
    StepEndSource = StepStartSource + N,
    StepTracksStartSource = StepEndSource + N,
    StepTracksEndSource = StepTracksStartSource + N,
    Count = StepTracksEndSource + N,
  };

  // How obsolete v1.1.0 parameter IDs map to v1.3 IDs
  enum class V110Params {
    LevelKnobs = enum_index(Param::StepEndLevel),
    ModeSwitches = enum_index(Param::StepTriggerMode),
    ConditionSwitches = enum_index(Param::StepInterruptsOnTrigger),
  };

  enum class Input {
    CurveSequencer,
    Gate,
    Loop,
    Reset,
    Run,
    EnableStep,
    Count = EnableStep + N,
  };

  enum class Output { CurveSequencer, Count };

  enum class Light { StepProgress, Count = StepProgress + N + N };

private:
  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = enum_index(Light::StepProgress, step + step);
    auto const remaining_light = completed_light + 1;
    lights_[completed_light].setBrightness(completed_brightness);
    lights_[remaining_light].setBrightness(remaining_brightness);
  }
};
} // namespace curve_sequencer_2
} // namespace dhe
