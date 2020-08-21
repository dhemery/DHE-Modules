#pragma once

#include "FuncControls.h"
#include "FuncEngine.h"
#include "OperandParamQuantity.h"

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
      configChannel(i);
    }
  }

  void process(ProcessArgs const & /*ignored*/) override {
    auto upstream = 0.F;
    for (auto i = 0; i < N; i++) {
      upstream = funcEngine.apply(i, upstream);
    }
  };

  void configChannel(int channel) {
    auto const channelName =
        N == 1 ? "" : std::string{" "} + std::to_string(channel + 1);
    configParam<OperandParamQuantity<Controls>>(Controls::AmountKnob + channel,
                                                0.F, 1.F, centeredRotation);
    configToggle<2>(this, Controls::OperationSwitch + channel,
                    "Operator" + channelName,
                    {"Add (offset)", "Multiply (scale)"}, 0);
    configToggle<4>(this, Controls::OffsetRangeSwitch + channel,
                    "Offset" + channelName + " range",
                    {"0–5 V", "±5 V", "0–10 V", "±10 V"}, 1);
    configToggle<4>(this, Controls::MultiplierRangeSwitch + channel,
                    "Multiplier" + channelName + " range",
                    {"0–1", "±1", "0–2", "±2"}, 2);

    auto const operandKnobParamQuantity =
        dynamic_cast<OperandParamQuantity<Controls> *>(
            paramQuantities[Controls::AmountKnob + channel]);

    operandKnobParamQuantity->configure(&controls, channel, channelName);
  }

private:
  Controls controls{inputs, params, outputs};
  FuncEngine<FuncControls, N> funcEngine{controls};
};
} // namespace func
} // namespace dhe
