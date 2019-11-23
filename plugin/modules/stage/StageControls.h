#pragma once

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

  class StageControls {
  public:
    StageControls(std::vector<Input> const &inputs, std::vector<Param> const &params, std::vector<Output> &outputs) :
        inputs{inputs}, params{params}, outputs{outputs} {}

    auto curvature() const -> float { return dhe::curvature(params[CurveKnob]); }

    auto duration() const -> float { return dhe::duration(params[DurationKnob], mediumDurationRange); }

    auto envelopeIn() const -> float { return voltageAt(inputs[EnvelopeInput]); }

    auto isTriggered() const -> bool { return isHigh(inputs[TriggerInput]); }

    auto isDeferring() const -> bool { return isHigh(inputs[DeferInput]); }

    auto level() const -> float { return dhe::level(params[LevelKnob], unipolarSignalRange); }

    void sendActive(bool active) {
      const auto voltage = unipolarSignalRange.scale(active);
      outputs[ActiveOutput].setVoltage(voltage);
    }

    void sendEoc(bool eoc) {
      const auto voltage = unipolarSignalRange.scale(eoc);
      outputs[EocOutput].setVoltage(voltage);
    }

    void sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

    auto taper() const -> taper::VariableTaper const * { return &taper::variableJTaper; };

    enum ParameterIds { DurationKnob, LevelKnob, CurveKnob, ParameterCount };

    enum InputIds { EnvelopeInput, TriggerInput, DeferInput, InputCount };

    enum OutputIds { EnvelopeOutput, EocOutput, ActiveOutput, OutputCount };

  private:
    std::vector<Input> const &inputs;
    std::vector<Param> const &params;
    std::vector<Output> &outputs;
  };
} // namespace stage
} // namespace dhe
