#pragma once

#include "Mode.h"
#include "controls/duration-inputs.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>

namespace dhe {
namespace stage {

using rack::engine::Input;
using rack::engine::Output;
using rack::engine::Param;

class HostageControls {
public:
  HostageControls(std::vector<Input> const &inputs,
                  std::vector<Param> const &params,
                  std::vector<Output> &outputs)
      : inputs{inputs}, params{params}, outputs{outputs} {};

  auto defer() const -> bool { return is_high(inputs[DeferInput]); }

  auto duration() const -> float {
    return selectable_duration(params[DurationKnob], inputs[DurationCvInput],
                               params[DurationRangeSwitch]);
  }

  auto gate() const -> bool { return is_high(inputs[TriggerInput]); }

  auto input() const -> float { return voltage_at(inputs[EnvelopeInput]); }

  auto mode() const -> Mode {
    return position_of(params[ModeSwitch]) == 1 ? Mode::Sustain : Mode::Hold;
  }

  void output(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  void showActive(bool active) {
    auto const voltage = unipolar_signal_range.scale(active);
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void showEoc(bool eoc) {
    auto const voltage = unipolar_signal_range.scale(eoc);
    outputs[EocOutput].setVoltage(voltage);
  }

  enum ParameterIds {
    DurationKnob,
    DurationRangeSwitch,
    ModeSwitch,
    ParameterCount
  };

  enum InputIds {
    DeferInput,
    DurationCvInput,
    EnvelopeInput,
    TriggerInput,
    InputCount
  };

  enum OutputIds { ActiveOutput, EnvelopeOutput, EocOutput, OutputCount };

private:
  std::vector<Input> const &inputs;
  std::vector<Param> const &params;
  std::vector<Output> &outputs;
};
} // namespace stage
} // namespace dhe
