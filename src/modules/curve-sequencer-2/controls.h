#pragma once

#include "components/sigmoid.h"
#include "controls/common-inputs.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"
#include "source.h"
#include "triggers.h"

#include <vector>

#define ENUMIDS(name, count) name, name##_LAST = (name) + (count)-1

namespace dhe {

namespace curve_sequencer_2 {

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

  // Step controls

  auto advance_on_end_of_curve(int step) const -> bool {
    return position_of(params_[OnEndOfCurveSwitches + step]) == 1;
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params_[CurveKnobs + step]);
  }

  auto duration(int step) const -> float {
    return dhe::selectable_duration(params_[DurationKnobs + step],
                                    params_[DurationRangeSwitch]);
  }

  auto end_level(int step) const -> float {
    return dhe::selectable_level(params_[EndLevelKnobs + step],
                                 params_[LevelRangeSwitch]);
  }

  auto end_source(int step) const -> Source {
    return static_cast<Source>(params_[EndSourceSwitches + step].getValue());
  }

  auto interrupt_on_trigger(int step) const -> bool {
    return position_of(params_[OnInterruptSwitches + step]) == 1;
  }

  void show_inactive(int step) { set_lights(step, 0.F, 0.F); }

  void show_progress(int step, float progress) {
    // Skew the progress::brightness ratio so that the "remaining" light stays
    // fully lit for a little while during early progress, and the "completed"
    // light reaches fully lit a little while before progress is complete.
    static auto constexpr brightness_skew = 0.7F;
    static auto constexpr brightness_range =
        Range{-brightness_skew, 1.F + brightness_skew};
    auto const completed_brightness = brightness_range.scale(progress);
    auto const remaining_brightness = 1.F - completed_brightness;
    set_lights(step, completed_brightness, remaining_brightness);
  }

  auto start_level(int step) const -> float {
    return dhe::selectable_level(params_[StartLevelKnobs + step],
                                 params_[LevelRangeSwitch]);
  }

  auto start_source(int step) const -> Source {
    return static_cast<Source>(params_[StartSourceSwitches + step].getValue());
  }

  auto taper(int step) const -> sigmoid::Taper const & {
    auto const selection =
        static_cast<int>(params_[ShapeSwitches + step].getValue());
    return sigmoid::tapers[selection];
  }

  auto track_end_source(int step) const -> bool {
    return position_of(params_[TrackEndSwitches + step]) == 1;
  }

  auto track_start_source(int step) const -> bool {
    return position_of(params_[TrackStartSwitches + step]) == 1;
  }

  auto trigger_mode(int step) const -> TriggerMode {
    return static_cast<TriggerMode>(
        params_[TriggerModeSwitches + step].getValue());
  }

  enum ParamIds {
    RunButton,
    GateButton,
    SelectionStartKnob,
    SelectionLengthKnob,
    LoopButton,
    ResetButton,
    DurationRangeSwitch,
    LevelRangeSwitch,
    ENUMIDS(CurveKnobs, N),
    ENUMIDS(DurationKnobs, N),
    ENUMIDS(EnabledButtons, N),
    ENUMIDS(EndLevelKnobs, N),       // Was LevelKnobs in v1.1.0
    ENUMIDS(TriggerModeSwitches, N), // Was ModeSwitches in v1.1.0
    ENUMIDS(OnInterruptSwitches, N), // Was ConditionSwitches in v1.1.0
    ENUMIDS(ShapeSwitches, N),
    ENUMIDS(StartLevelKnobs, N), // Was GenerateModeMenu in v1.1.0?
    // The rest are new in 1.3.0
    ENUMIDS(OnEndOfCurveSwitches, N),
    ENUMIDS(StartSourceSwitches, N),
    ENUMIDS(EndSourceSwitches, N),
    ENUMIDS(TrackStartSwitches, N),
    ENUMIDS(TrackEndSwitches, N),
    ParamCount
  };

  enum InputIds {
    CurveSequencerInput,
    GateInput,
    LoopInput,
    ResetInput,
    RunInput,
    ENUMIDS(EnabledInputs, N),
    InputCount
  };

  enum OutputIds { CurveSequencerOutput, OutputCount };

  enum LightIds { ENUMIDS(ProgressLights, N * 2), LightCount };

private:
  void set_lights(int step, float completed_brightness,
                  float remaining_brightness) {
    auto const completed_light = ProgressLights + step + step;
    auto const remaining_light = completed_light + 1;
    lights_[completed_light].setBrightness(completed_brightness);
    lights_[remaining_light].setBrightness(remaining_brightness);
  }
};
} // namespace curve_sequencer_2
} // namespace dhe
