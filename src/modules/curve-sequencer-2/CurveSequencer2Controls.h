#pragma once

#include "TriggerMode.h"
#include "controls/CommonInputs.h"
#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"

#include <vector>

#define ENUMIDS(name, count) name, name##_LAST = name + (count) -1

namespace dhe {

namespace curve_sequencer {

  template <typename PortT, typename ParamT, typename LightT, int N> class CurveSequencer2Controls {
  private:
    std::vector<PortT> &inputs;
    std::vector<PortT> &outputs;
    std::vector<ParamT> &params;
    std::vector<LightT> &lights;

  public:
    CurveSequencer2Controls(std::vector<PortT> &inputs, std::vector<PortT> &outputs, std::vector<ParamT> &params,
                            std::vector<LightT> &lights) :
        inputs{inputs}, outputs{outputs}, params{params}, lights{lights} {}

    // Sequence Controls

    auto input() const -> float { return inputs[CurveSequencerInput].getVoltage(); }

    auto isEnabled(int step) const -> bool {
      return isPressed(params[EnabledButtons + step]) || isHigh(inputs[EnabledInputs + step]);
    }

    auto isGated() const -> bool { return isHigh(inputs[GateInput]) || isPressed(params[GateButton]); }

    auto isLooping() const -> bool { return isPressed(params[LoopButton]) || isHigh(inputs[LoopInput]); }

    auto isReset() const -> bool { return isHigh(inputs[ResetInput]) || isPressed(params[ResetButton]); }

    auto isRunning() const -> bool { return isPressed(params[RunButton]) || isHigh(inputs[RunInput]); }

    auto output() const -> float { return outputs[CurveSequencerOutput].getVoltage(); }

    void output(float voltage) { outputs[CurveSequencerOutput].setVoltage(voltage); }

    auto selectionStart() const -> int { return static_cast<int>(params[SelectionStartKnob].getValue()); }

    auto selectionLength() const -> int { return static_cast<int>(params[SelectionLengthKnob].getValue()); }

    auto triggerMode(int step) const -> TriggerMode {
      return static_cast<TriggerMode>(params[TriggerModePickers + step].getValue());
    }

    auto curvature(int step) const -> float { return dhe::curvature(params[CurveKnobs + step]); }

    auto duration(int step) const -> float {
      return dhe::selectableDuration(params[DurationKnobs + step], params[DurationRangeSwitch]);
    }

    auto level(int step) const -> float {
      return dhe::selectableLevel(params[LevelKnobs + step], params[LevelRangeSwitch]);
    }

    void showInactive(int step) { setLights(step, 0.F, 0.F); }

    void showProgress(int step, float progress) {
      // Skew the progress::brightness ratio so that the "remaining" light stays fully lit for a little while during
      // early progress, and the "completed" light reaches fully lit a little while before progress is complete.
      static auto constexpr brightnessSkew = 0.7F;
      static auto constexpr brightnessRange = Range{-brightnessSkew, 1.F + brightnessSkew};
      auto const completedBrightness = brightnessRange.scale(progress);
      auto const remainingBrightness = 1.F - completedBrightness;
      setLights(step, completedBrightness, remainingBrightness);
    }

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
      ENUMIDS(CurveKnobs, N),
      ENUMIDS(DurationKnobs, N),
      ENUMIDS(EnabledButtons, N),
      ENUMIDS(LevelKnobs, N),
      ENUMIDS(TriggerModePickers, N),
      ENUMIDS(ShapeSwitches, N),
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
    void setLights(int step, float completedBrightness, float remainingBrightness) {
      auto const completedLight = ProgressLights + step + step;
      auto const remainingLight = completedLight + 1;
      lights[completedLight].setBrightness(completedBrightness);
      lights[remainingLight].setBrightness(remainingBrightness);
    }
  };
} // namespace curve_sequencer

} // namespace dhe
