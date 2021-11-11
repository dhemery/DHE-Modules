#pragma once
#include "control-ids.h"
#include "engine.h"
#include "params.h"
#include "signals.h"

#include "params/presets.h"

#include "rack.hpp"

namespace dhe {
namespace func {

template <int N> struct Module : public rack::engine::Module {
  using Input = InputIds<N>;
  using Param = ParamIds<N>;
  using Output = OutputIds<N>;

  Module() {
    config(Param::Count, Input::Count, Output::Count);

    for (auto i = 0; i < N; i++) {
      config_channel(i);
    }
  }

  void process(ProcessArgs const & /*ignored*/) override {
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
    static auto const offset_knob_name =
        channel_name + (N == 1 ? "Offset" : " offset");
    static auto const multiplier_knob_name =
        channel_name + (N == 1 ? "Multiplier" : " multiplier");
    static auto const operand_knob_names =
        std::vector<std::string>{offset_knob_name, multiplier_knob_name};

    auto *operand_knob = Knob::config<Operations>(
        this, Param::Operand + channel, offset_knob_name);

    auto select_operation = [operand_knob](Operations::Selection selection) {
      operand_knob->mapper().select_operation(selection);
      if (selection == Operations::Multiply) {
        operand_knob->unit = "";
        operand_knob->name = multiplier_knob_name;
      } else {
        operand_knob->unit = " V";
        operand_knob->name = offset_knob_name;
      }
    };
    auto select_multiplier_range =
        [operand_knob](Multipliers::Selection selection) {
          operand_knob->mapper().select_multiplier_range(selection);
        };
    auto select_offset_range = [operand_knob](Addends::Selection selection) {
      operand_knob->mapper().select_offset_range(selection);
    };

    auto const operator_switch_name =
        channel_name + (N == 1 ? "Operator" : " operator");
    Switch::config<Operations>(this, Param::Operation + channel,
                               operator_switch_name, Operations::Add)
        ->on_change(select_operation);

    auto const offset_range_switch_name =
        channel_name + (N == 1 ? "Offset range" : " addend range");
    Switch::config<Addends>(this, Param::AddendRange + channel,
                            offset_range_switch_name, Addends::Bipolar)
        ->on_change(select_offset_range);

    auto const multiplier_range_switch_name =
        channel_name + (N == 1 ? "Multiplier range" : " multiplier range");
    Switch::config<Multipliers>(this, Param::MultiplierRange + channel,
                                multiplier_range_switch_name, Multipliers::Gain)
        ->on_change(select_multiplier_range);

    auto const port_name = N == 1 ? "Func" : channel_name;
    configInput(Input::Channel + channel, port_name);
    configOutput(Output::Channel + channel, port_name);
  }

  using RackSignals = Signals<rack::engine::Param, rack::engine::Input,
                              rack::engine::Output, N>;
  RackSignals signals_{params, inputs, outputs};
  FuncEngine<RackSignals> func_engine_{signals_};
};
} // namespace func
} // namespace dhe
