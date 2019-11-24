#pragma once

#include "components/Taper.h"
#include "controls/CommonInputs.h"
#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace stage {
  using rack::engine::Input;
  using rack::engine::Output;
  using rack::engine::Param;

  class BoosterStageControls {
  public:
    BoosterStageControls(std::vector<Input> const &inputs, std::vector<Param> const &params,
                         std::vector<Output> &outputs) :
        inputs{inputs}, params{params}, outputs{outputs} {}

    auto curvature() const -> float { return dhe::curvature(params[CurveKnob], inputs[CurveCvInput]); }

    auto duration() const -> float {
      return selectableDuration(params[DurationKnob], inputs[DurationCvInput], params[DurationRangeSwitch]);
    }

    auto input() const -> float { return voltageAt(inputs[EnvelopeInput]); }

    auto isDeferring() const -> bool { return isHigh(inputs[DeferInput]) || isPressed(params[DeferButton]); }

    auto isTriggered() const -> bool { return isHigh(inputs[TriggerInput]) || isPressed(params[TriggerButton]); }

    auto level() const -> float {
      return selectableLevel(params[LevelKnob], inputs[LevelCvInput], params[LevelRangeSwitch]);
    }

    void output(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

    void sendActive(bool isActive) {
      auto const voltage = unipolarSignalRange.scale(isActive || activeButton());
      outputs[ActiveOutput].setVoltage(voltage);
    }

    void sendEoc(bool isEoc) {
      auto const voltage = unipolarSignalRange.scale(isEoc || eocButton());
      outputs[EocOutput].setVoltage(voltage);
    }

    auto taper() const -> taper::VariableTaper const * { return selectedTaper(params[ShapeSwitch]); }

    enum ParameterIds {
      ActiveButton,
      CurveKnob,
      DeferButton,
      DurationKnob,
      DurationRangeSwitch,
      EocButton,
      LevelKnob,
      LevelRangeSwitch,
      ShapeSwitch,
      TriggerButton,
      ParameterCount
    };

    enum InputIds { CurveCvInput, DeferInput, DurationCvInput, LevelCvInput, EnvelopeInput, TriggerInput, InputCount };

    enum OutputIds { ActiveOutput, EocOutput, EnvelopeOutput, OutputCount };

  private:
    auto activeButton() const -> bool { return isPressed(params[ActiveButton]); }

    auto eocButton() const -> bool { return isPressed(params[EocButton]); }

    std::vector<Input> const &inputs;
    std::vector<Param> const &params;
    std::vector<Output> &outputs;
  };
} // namespace stage
} // namespace dhe
