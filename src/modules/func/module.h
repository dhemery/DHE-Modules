#pragma once
#include "control-ids.h"
#include "controls.h"
#include "engine.h"
#include "multiplier.h"
#include "offset.h"
#include "operation.h"
#include "signals.h"

#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/presets.h"

#include "rack.hpp"

namespace dhe {
namespace func {

template <int N> struct Module : public rack::engine::Module {
  using InputId = InputIds<N>;
  using ParamId = ParamIds<N>;
  using OutputId = OutputIds<N>;

  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    for (auto i = 0; i < N; i++) {
      config_channel(i);
    }
  }

  void process(ProcessArgs const & /*args*/) override {
    auto upstream = 0.F;
    for (auto i = 0; i < N; i++) {
      upstream = func_engine_.apply(i, upstream);
    }
  };

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  void config_channel(int channel) {
    auto const channel_name =
        N == 1 ? std::string{""}
               : std::string{"Channel "} + std::to_string(channel + 1);
    auto const offset_knob_name =
        channel_name + (N == 1 ? "Offset" : " offset");
    auto const multiplier_knob_name =
        channel_name + (N == 1 ? "Multiplier" : " multiplier");

    auto *operand_knob = OperandKnob::config(this, ParamId::Operand + channel)
                             ->set_offset_knob_name(offset_knob_name)
                             ->set_multiplier_knob_name(multiplier_knob_name);

    auto const offset_range_switch_name =
        channel_name + (N == 1 ? "Offset range" : " offset range");
    OffsetRangeStepper::config(this, ParamId::OffsetRange + channel,
                               offset_range_switch_name)
        ->set_operand_knob(operand_knob);

    auto const multiplier_range_switch_name =
        channel_name + (N == 1 ? "Multiplier range" : " multiplier range");
    MultiplierRangeStepper::config(this, ParamId::MultiplierRange + channel,
                                   multiplier_range_switch_name)
        ->set_operand_knob(operand_knob);

    auto const operator_switch_name =
        channel_name + (N == 1 ? "Operator" : " operator");
    OperationSwitch::config(this, ParamId::Operation + channel,
                            operator_switch_name)
        ->set_operand_knob(operand_knob);

    auto const port_name = N == 1 ? "Func" : channel_name;
    configInput(InputId::Channel + channel, port_name);
    configOutput(OutputId::Channel + channel, port_name);
  }

  using RackSignals = Signals<rack::engine::Param, rack::engine::Input,
                              rack::engine::Output, N>;
  RackSignals signals_{params, inputs, outputs};
  FuncEngine<RackSignals> func_engine_{signals_};
};
} // namespace func
} // namespace dhe
