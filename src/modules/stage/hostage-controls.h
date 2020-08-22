#pragma once

#include "controls/duration-inputs.h"
#include "mode.h"

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
      : inputs_{inputs}, params_{params}, outputs_{outputs} {};

  auto defer() const -> bool { return is_high(inputs_[DeferInput]); }

  auto duration() const -> float {
    return selectable_duration(params_[DurationKnob], inputs_[DurationCvInput],
                               params_[DurationRangeSwitch]);
  }

  auto gate() const -> bool { return is_high(inputs_[TriggerInput]); }

  auto input() const -> float { return voltage_at(inputs_[EnvelopeInput]); }

  auto mode() const -> Mode {
    return position_of(params_[ModeSwitch]) == 1 ? Mode::Sustain : Mode::Hold;
  }

  void output(float voltage) { outputs_[EnvelopeOutput].setVoltage(voltage); }

  void show_active(bool active) {
    auto const voltage = unipolar_signal_range.scale(active);
    outputs_[ActiveOutput].setVoltage(voltage);
  }

  void show_eoc(bool eoc) {
    auto const voltage = unipolar_signal_range.scale(eoc);
    outputs_[EocOutput].setVoltage(voltage);
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
  std::vector<Input> const &inputs_;
  std::vector<Param> const &params_;
  std::vector<Output> &outputs_;
};
} // namespace stage
} // namespace dhe
