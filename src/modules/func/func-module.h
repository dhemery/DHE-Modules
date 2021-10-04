#pragma once

#include "func-controls.h"
#include "func-engine.h"
#include "operand-param-quantity.h"

#include "config/common-config.h"
#include <engine/Module.hpp>
#include <vector>

namespace dhe {
namespace func {

template <int N> class FuncModule : public rack::engine::Module {
  using Controls = FuncControls<N>;

public:
  FuncModule() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

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
    configParam<OperandParamQuantity<FuncControls, N>>(
        Controls::AmountKnob + channel, 0.F, 1.F, centered_rotation);
    auto const channel_name =
        N == 1 ? std::string{""}
               : std::string{"Channel "} + std::to_string(channel + 1);

    auto const operator_switch_name =
        channel_name + (N == 1 ? "Operator" : " operator");
    config_switch(this, Controls::OperationSwitch + channel,
                  operator_switch_name, {"Add (offset)", "Multiply (scale)"},
                  0);

    auto const offset_range_switch_name =
        channel_name + (N == 1 ? "Offset range" : " offset range");
    config_switch(this, Controls::OffsetRangeSwitch + channel,
                  offset_range_switch_name,
                  {"0–5 V", "±5 V", "0–10 V", "±10 V"}, 1);

    auto const multiplier_range_switch_name =
        channel_name + (N == 1 ? "Multiplier range" : " multiplier range");
    config_switch(this, Controls::MultiplierRangeSwitch + channel,
                  multiplier_range_switch_name, {"0–1", "±1", "0–2", "±2"}, 2);

    auto const operand_knob_param_quantity =
        dynamic_cast<OperandParamQuantity<FuncControls, N> *>(
            paramQuantities[Controls::AmountKnob + channel]);

    operand_knob_param_quantity->configure(&controls_, channel, channel_name);

    auto const port_name = N == 1 ? "Func" : channel_name;
    configInput(Controls::FuncInput + channel, port_name);
    configOutput(Controls::FuncOutput + channel, port_name);
  }

  Controls controls_{inputs, params, outputs};
  FuncEngine<FuncControls, N> func_engine_{controls_};
};
} // namespace func
} // namespace dhe
