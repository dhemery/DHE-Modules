#pragma once

#include "StepCondition.h"
#include "StepMode.h"
#include "controls/CommonInputs.h"
#include "controls/CurvatureInputs.h"

#include <common.hpp>
#include <controls/DurationInputs.h>
#include <controls/LevelInputs.h>
#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {

namespace curve_sequencer {

  using rack::engine::Input;
  using rack::engine::Light;
  using rack::engine::Output;
  using rack::engine::Param;

  template <int N> class CurveSequencerControls {
  private:
    std::vector<Input> &inputs;
    std::vector<Output> &outputs;
    std::vector<Param> &params;
    std::vector<Light> &lights;

  public:
    CurveSequencerControls(std::vector<Input> &inputs, std::vector<Output> &outputs, std::vector<Param> &params,
                           std::vector<Light> &lights) :
        inputs{inputs}, outputs{outputs}, params{params}, lights{lights} {}

    auto condition(int step) const -> StepCondition {
      return static_cast<StepCondition>(params[ConditionSwitches + step].getValue());
    }

    auto curvature(int step) const -> float { return dhe::curvature(params[CurveKnobs + step]); }

    auto duration(int step) const -> float {
      return dhe::selectableDuration(params[DurationKnobs + step], params[DurationRangeSwitch]);
    }

    auto input() const -> float { return inputs[CurveSequencerInput].getVoltage(); }

    auto isEnabled(int step) const -> bool {
      return isPressed(params[EnabledButtons + step]) || isHigh(inputs[EnabledInputs + step]);
    }

    auto isGated() const -> bool { return isHigh(inputs[GateInput]) || isPressed(params[GateButton]); }

    auto isLooping() const -> bool { return isPressed(params[LoopButton]) || isHigh(inputs[LoopInput]); }

    auto isReset() const -> bool { return isHigh(inputs[ResetInput]) || isPressed(params[ResetButton]); }

    auto isRunning() const -> bool { return isPressed(params[RunButton]) || isHigh(inputs[RunInput]); }

    auto level(int step) const -> float {
      return dhe::selectableLevel(params[LevelKnobs + step], params[LevelRangeSwitch]);
    }

    auto mode(int step) const -> StepMode { return static_cast<StepMode>(params[ModeSwitches + step].getValue()); }

    auto output() const -> float { return outputs[CurveSequencerOutput].getVoltage(); }

    void output(float voltage) { outputs[CurveSequencerOutput].setVoltage(voltage); }

    auto selectionStart() const -> int { return static_cast<int>(params[SelectionStartKnob].getValue()); }

    auto selectionLength() const -> int { return static_cast<int>(params[SelectionLengthKnob].getValue()); }

    void showActive(int step, bool state) { lights[ActivityLights + step].setBrightness(state ? 10.F : 0.F); }

    auto taper(int step) const -> taper::VariableTaper const * {
      auto const selection = static_cast<int>(params[ShapeSwitches + step].getValue());
      return taper::variableTapers[selection];
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
      ENUMS(CurveKnobs, N),
      ENUMS(DurationKnobs, N),
      ENUMS(EnabledButtons, N),
      ENUMS(LevelKnobs, N),
      ENUMS(ModeSwitches, N),
      ENUMS(ConditionSwitches, N),
      ENUMS(ShapeSwitches, N),
      ParameterCount
    };

    enum InputIds {
      CurveSequencerInput,
      GateInput,
      LoopInput,
      ResetInput,
      RunInput,
      ENUMS(EnabledInputs, N),
      InputCount
    };

    enum OutputIds { CurveSequencerOutput, OutputCount };

    enum LightIds { ENUMS(ActivityLights, N), LightCount };
  };

} // namespace curve_sequencer

} // namespace dhe
