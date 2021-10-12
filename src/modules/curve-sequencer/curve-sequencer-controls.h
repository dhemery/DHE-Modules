#pragma once

#include "./advance-mode.h"
#include "./generate-mode.h"
#include "controls/common-inputs.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"

#include <vector>

#define PER_STEP(name, n) name, name##_LAST = (name) + (n)-1
#define ONE_PER_STEP(name, n) PER_STEP(name, n)
#define TWO_PER_STEP(name, n) PER_STEP(name, (n)*2)

namespace dhe {

namespace curve_sequencer {

static auto constexpr brightness_skew = 0.7F;
static auto constexpr brightness_range =
    Range{-brightness_skew, 1.F + brightness_skew};

template <typename Inputs, typename Outputs, typename Params, typename Lights,
          int N>
class CurveSequencerControls {
private:
  Inputs &inputs_;
  Outputs &outputs_;
  Params &params_;
  Lights &lights_;

public:
  CurveSequencerControls(Inputs &inputs, Outputs &outputs, Params &params,
                         Lights &lights)
      : inputs_{inputs}, outputs_{outputs}, params_{params}, lights_{lights} {}

  auto condition(int step) const -> AdvanceMode {
    return static_cast<AdvanceMode>(
        params_[ConditionSwitches + step].getValue());
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params_[CurveKnobs + step]);
  }

  auto duration(int step) const -> float {
    return dhe::selectable_duration(params_[DurationKnobs + step],
                                    params_[DurationRangeSwitch]);
  }

  auto input() const -> float {
    return inputs_[CurveSequencerInput].getVoltage();
  }

  auto is_enabled(int step) const -> bool {
    return is_pressed(params_[EnabledButtons + step]) ||
           is_high(inputs_[EnabledInputs + step]);
  }

  auto is_gated() const -> bool {
    return is_high(inputs_[GateInput]) || is_pressed(params_[GateButton]);
  }

  auto is_looping() const -> bool {
    return is_pressed(params_[LoopButton]) || is_high(inputs_[LoopInput]);
  }

  auto is_reset() const -> bool {
    return is_high(inputs_[ResetInput]) || is_pressed(params_[ResetButton]);
  }

  auto is_running() const -> bool {
    return is_pressed(params_[RunButton]) || is_high(inputs_[RunInput]);
  }

  auto level(int step) const -> float {
    return dhe::selectable_level(params_[LevelKnobs + step],
                                 params_[LevelRangeSwitch]);
  }

  auto mode(int step) const -> GenerateMode {
    return static_cast<GenerateMode>(params_[ModeSwitches + step].getValue());
  }

  auto output() const -> float {
    return outputs_[CurveSequencerOutput].getVoltage();
  }

  void output(float voltage) {
    outputs_[CurveSequencerOutput].setVoltage(voltage);
  }

  auto selection_start() const -> int {
    return static_cast<int>(params_[SelectionStartKnob].getValue());
  }

  auto selection_length() const -> int {
    return static_cast<int>(params_[SelectionLengthKnob].getValue());
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
        static_cast<int>(params_[ShapeSwitches + step].getValue());
    return sigmoid::tapers[selection];
  }

  enum ParameterIds {
    RunButton,
    GateButton,
    SelectionStartKnob,
    SelectionLengthKnob,
    LoopButton,
    ResetButton,
    DurationRangeSwitch,
    LevelRangeSwitch,
    ONE_PER_STEP(CurveKnobs, N),
    ONE_PER_STEP(DurationKnobs, N),
    ONE_PER_STEP(EnabledButtons, N),
    ONE_PER_STEP(LevelKnobs, N),
    ONE_PER_STEP(ModeSwitches, N),
    ONE_PER_STEP(ConditionSwitches, N),
    ONE_PER_STEP(ShapeSwitches, N),
    GenerateModeMenu,
    ParameterCount
  };

  enum InputIds {
    CurveSequencerInput,
    GateInput,
    LoopInput,
    ResetInput,
    RunInput,
    ONE_PER_STEP(EnabledInputs, N),
    InputCount
  };

  enum OutputIds { CurveSequencerOutput, OutputCount };

  enum LightIds { TWO_PER_STEP(ProgressLights, N), LightCount };

private:
  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = ProgressLights + step + step;
    auto const remaining_light = completed_light + 1;
    lights_[completed_light].setBrightness(completed_brightness);
    lights_[remaining_light].setBrightness(remaining_brightness);
  }
};
} // namespace curve_sequencer

} // namespace dhe

#undef PER_STEP
#undef ONE_PER_STEP
#undef TWO_PER_STEP
