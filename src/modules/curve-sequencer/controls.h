#pragma once

#include "anchor.h"
#include "components/sigmoid.h"
#include "controls/common-inputs.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"
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

public:
  Controls(std::vector<InputT> &inputs, std::vector<OutputT> &outputs,
           std::vector<ParamT> &params, std::vector<LightT> &lights)
      : inputs_{inputs}, outputs_{outputs}, params_{params}, lights_{lights} {}

  // Sequence Controls

  auto input() const -> float { return input(Input::In).getVoltage(); }

  auto is_enabled(int step) const -> bool {
    return is_pressed(param(Param::EnableStep, step)) ||
           is_high(input(Input::EnableStep, step));
  }

  auto is_gated() const -> bool {
    return is_high(input(Input::Gate)) || is_pressed(param(Param::Gate));
  }

  auto is_looping() const -> bool {
    return is_pressed(param(Param::Loop)) || is_high(input(Input::Loop));
  }

  auto is_reset() const -> bool {
    return is_high(input(Input::Reset)) || is_pressed(param(Param::Reset));
  }

  auto is_running() const -> bool {
    return is_pressed(param(Param::Run)) || is_high(input(Input::Run));
  }

  auto output() const -> float { return outputs_[Output::Out].getVoltage(); }

  void output(float voltage) { outputs_[Output::Out].setVoltage(voltage); }

  auto selection_start() const -> int {
    return static_cast<int>(param(Param::SelectionStart).getValue());
  }

  auto selection_length() const -> int {
    return static_cast<int>(param(Param::SelectionLength).getValue());
  }

  // Step controls

  auto advance_on_end_of_curve(int step) const -> bool {
    return position_of(param(Param::StepCompletionMode, step)) == 1;
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(param(Param::StepCurvature, step));
  }

  auto duration(int step) const -> float {
    return dhe::selectable_duration(param(Param::StepDuration, step),
                                    param(Param::DurationRange));
  }

  auto end_level(int step) const -> float {
    return dhe::selectable_level(param(Param::StepEndLevel, step),
                                 param(Param::LevelRange));
  }

  auto end_source(int step) const -> AnchorMode {
    return static_cast<AnchorMode>(
        param(Param::StepEndAnchorSource, step).getValue());
  }

  auto interrupt_on_trigger(int step) const -> bool {
    return position_of(param(Param::StepInterruptMode, step)) == 1;
  }

  void show_inactive(int step) { set_lights(step, 0.F, 0.F); }

  void show_progress(int step, float progress) {
    auto const completed_brightness = brightness_range.scale(progress);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
  }

  auto start_level(int step) const -> float {
    return dhe::selectable_level(param(Param::StepStartLevel, step),
                                 param(Param::LevelRange));
  }

  auto start_source(int step) const -> AnchorMode {
    return static_cast<AnchorMode>(
        param(Param::StepStartAnchorSource, step).getValue());
  }

  auto taper(int step) const -> sigmoid::Taper const & {
    auto const selection =
        static_cast<int>(param(Param::StepShape, step).getValue());
    return sigmoid::tapers[selection];
  }

  auto track_end_source(int step) const -> bool {
    return position_of(param(Param::StepEndAnchorMode, step)) == 1;
  }

  auto track_start_source(int step) const -> bool {
    return position_of(param(Param::StepStartAnchorMode, step)) == 1;
  }

  auto trigger_mode(int step) const -> TriggerMode {
    return static_cast<TriggerMode>(
        param(Param::StepTriggerMode, step).getValue());
  }

  struct Param {
    enum {
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
      StepInterruptMode = StepTriggerMode + N,
      StepShape = StepInterruptMode + N,
      // The rest are new in 1.3.0
      StepStartLevel = StepShape + N,
      StepCompletionMode = StepStartLevel + N,
      StepStartAnchorSource = StepCompletionMode + N,
      StepEndAnchorSource = StepStartAnchorSource + N,
      StepStartAnchorMode = StepEndAnchorSource + N,
      StepEndAnchorMode = StepStartAnchorMode + N,
      Count = StepEndAnchorMode + N,
    };
  };

  // How obsolete v1.1.0 parameter IDs map to v1.3 IDs
  struct V110Params {
    enum {
      LevelKnobs = enum_index(Param::StepEndLevel),
      ModeSwitches = enum_index(Param::StepTriggerMode),
      ConditionSwitches = enum_index(Param::StepInterruptMode),
    };
  };

  struct Input {
    enum {
      In,
      Gate,
      Loop,
      Reset,
      Run,
      EnableStep,
      Count = EnableStep + N,
    };
  };

  struct Output {
    enum {
      Out,
      Count,
    };
  };

  struct Light {
    enum {
      StepProgress,
      Count = StepProgress + N + N,
    };
  };

private:
  auto input(int id, int offset = 0) const -> InputT & {
    return inputs_[id + offset];
  }

  auto param(int id, int offset = 0) const -> ParamT & {
    return params_[id + offset];
  }

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
