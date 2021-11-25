#pragma once
#include "control-ids.h"
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

    auto *operand_knob = Knob::config<Operand>(this, ParamId::Operand + channel,
                                               offset_knob_name);

    auto select_operation = [operand_knob, multiplier_knob_name,
                             offset_knob_name](Operation op) {
      operand_knob->mapper().select_operation(op);
      if (op == Operation::Multiply) {
        operand_knob->unit = MultiplierRanges::unit;
        operand_knob->name = multiplier_knob_name;
      } else {
        operand_knob->unit = OffsetRanges::unit;
        operand_knob->name = offset_knob_name;
      }
    };
    auto select_multiplier_range = [operand_knob](MultiplierRangeId id) {
      operand_knob->mapper().select_multiplier_range(id);
    };
    auto select_offset_range = [operand_knob](OffsetRangeId id) {
      operand_knob->mapper().select_offset_range(id);
    };

    auto const operator_switch_name =
        channel_name + (N == 1 ? "Operator" : " operator");
    Switch::config<Operations>(this, ParamId::Operation + channel,
                               operator_switch_name, Operation::Add)
        ->on_change(select_operation);

    auto const offset_range_switch_name =
        channel_name + (N == 1 ? "Offset range" : " offset range");
    Switch::config<OffsetRanges>(this, ParamId::OffsetRange + channel,
                                 offset_range_switch_name,
                                 OffsetRangeId::Bipolar)
        ->on_change(select_offset_range);

    auto const multiplier_range_switch_name =
        channel_name + (N == 1 ? "Multiplier range" : " multiplier range");
    Switch::config<MultiplierRanges>(this, ParamId::MultiplierRange + channel,
                                     multiplier_range_switch_name,
                                     MultiplierRangeId::Gain)
        ->on_change(select_multiplier_range);

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
