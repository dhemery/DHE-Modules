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
    configParam<OperandParamQuantity<RackSignals>>(Param::Operand + channel,
                                                   0.F, 1.F, 0.5F);
    auto const channel_name =
        N == 1 ? std::string{""}
               : std::string{"Channel "} + std::to_string(channel + 1);

    auto const operator_switch_name =
        channel_name + (N == 1 ? "Operator" : " operator");
    Switch::config<Operators>(this, Param::Operation + channel,
                              operator_switch_name, Operation::Add);

    auto const offset_range_switch_name =
        channel_name + (N == 1 ? "Offset range" : " offset range");
    Switch::config<OffsetRanges>(this, Param::OffsetRange + channel,
                                 offset_range_switch_name, 1);

    auto const multiplier_range_switch_name =
        channel_name + (N == 1 ? "Multiplier range" : " multiplier range");
    Switch::config<MultiplierRanges>(this, Param::MultiplierRange + channel,
                                     multiplier_range_switch_name, 2);

    auto const operand_knob_param_quantity =
        reinterpret_cast<OperandParamQuantity<RackSignals> *>(
            getParamQuantity(Param::Operand + channel));

    operand_knob_param_quantity->configure(&signals_, channel, channel_name);

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
