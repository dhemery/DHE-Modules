#pragma once

#include "AdvanceMode.h"
#include "GenerateMode.h"
#include "controls/CommonInputs.h"
#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"

#include <vector>

#define ENUMIDS(name, count) name, name##_LAST = (name) + (count)-1

namespace dhe {

namespace curve_sequencer {

template <typename InputT, typename OutputT, typename ParamT, typename LightT,
          int N>
class CurveSequencerControls {
private:
  std::vector<InputT> &inputs;
  std::vector<OutputT> &outputs;
  std::vector<ParamT> &params;
  std::vector<LightT> &lights;

public:
  CurveSequencerControls(std::vector<InputT> &inputs,
                         std::vector<OutputT> &outputs,
                         std::vector<ParamT> &params,
                         std::vector<LightT> &lights)
      : inputs{inputs}, outputs{outputs}, params{params}, lights{lights} {}

  auto condition(int step) const -> AdvanceMode {
    return static_cast<AdvanceMode>(
        params[ConditionSwitches + step].getValue());
  }

  auto curvature(int step) const -> float {
    return dhe::curvature(params[CurveKnobs + step]);
  }

  auto duration(int step) const -> float {
    return dhe::selectable_duration(params[DurationKnobs + step],
                                    params[DurationRangeSwitch]);
  }

  auto input() const -> float {
    return inputs[CurveSequencerInput].getVoltage();
  }

  auto isEnabled(int step) const -> bool {
    return is_pressed(params[EnabledButtons + step]) ||
           is_high(inputs[EnabledInputs + step]);
  }

  auto isGated() const -> bool {
    return is_high(inputs[GateInput]) || is_pressed(params[GateButton]);
  }

  auto isLooping() const -> bool {
    return is_pressed(params[LoopButton]) || is_high(inputs[LoopInput]);
  }

  auto isReset() const -> bool {
    return is_high(inputs[ResetInput]) || is_pressed(params[ResetButton]);
  }

  auto isRunning() const -> bool {
    return is_pressed(params[RunButton]) || is_high(inputs[RunInput]);
  }

  auto level(int step) const -> float {
    return dhe::selectableLevel(params[LevelKnobs + step],
                                params[LevelRangeSwitch]);
  }

  auto mode(int step) const -> GenerateMode {
    return static_cast<GenerateMode>(params[ModeSwitches + step].getValue());
  }

  auto output() const -> float {
    return outputs[CurveSequencerOutput].getVoltage();
  }

  void output(float voltage) {
    outputs[CurveSequencerOutput].setVoltage(voltage);
  }

  auto selectionStart() const -> int {
    return static_cast<int>(params[SelectionStartKnob].getValue());
  }

  auto selectionLength() const -> int {
    return static_cast<int>(params[SelectionLengthKnob].getValue());
  }

  void showInactive(int step) { setLights(step, 0.F, 0.F); }

  void showProgress(int step, float progress) {
    // Skew the progress::brightness ratio so that the "remaining" light stays
    // fully lit for a little while during early progress, and the "completed"
    // light reaches fully lit a little while before progress is complete.
    static auto constexpr brightnessSkew = 0.7F;
    static auto constexpr brightnessRange =
        Range{-brightnessSkew, 1.F + brightnessSkew};
    auto const completedBrightness = brightnessRange.scale(progress);
    auto const remainingBrightness = 1.F - completedBrightness;
    setLights(step, completedBrightness, remainingBrightness);
  }

  auto taper(int step) const -> taper::VariableTaper const * {
    auto const selection =
        static_cast<int>(params[ShapeSwitches + step].getValue());
    return taper::variable_tapers[selection];
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
    ENUMIDS(CurveKnobs, N),
    ENUMIDS(DurationKnobs, N),
    ENUMIDS(EnabledButtons, N),
    ENUMIDS(LevelKnobs, N),
    ENUMIDS(ModeSwitches, N),
    ENUMIDS(ConditionSwitches, N),
    ENUMIDS(ShapeSwitches, N),
    GenerateModeMenu,
    ParameterCount
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
  void setLights(int step, float completedBrightness,
                 float remainingBrightness) {
    auto const completedLight = ProgressLights + step + step;
    auto const remainingLight = completedLight + 1;
    lights[completedLight].setBrightness(completedBrightness);
    lights[remainingLight].setBrightness(remainingBrightness);
  }
};
} // namespace curve_sequencer

} // namespace dhe
