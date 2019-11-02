#pragma once

#include "Func.h"
#include "FuncOperandKnobParamQuantity.h"

#include <engine/Module.hpp>
#include <vector>

namespace dhe {
namespace func {

  template <int N> class FuncModule : public rack::engine::Module {
  public:
    FuncModule() {
      config(FuncControls<N>::ParameterCount, FuncControls<N>::InputCount, FuncControls<N>::OutputCount);

      for (int i = 0; i < N; i++) {
        configChannel(i);
      }
    }

    void process(const ProcessArgs & /*ignored*/) override {
      auto upstream = 0.F;
      for (auto i = 0; i < N; i++) {
        upstream = channel.apply(i, upstream);
      }
    };

    void configChannel(int channel) {
      auto const channelName = N == 1 ? "" : std::string{" "} + std::to_string(channel + 1);
      configParam<FuncOperandKnobParamQuantity>(FuncControls<N>::AmountKnob + channel, 0.F, 1.F, centeredRotation);
      configToggle<2>(this, FuncControls<N>::OperationSwitch + channel, "Operator" + channelName,
                      {"Add (offset)", "Multiply (scale)"}, 0);
      configToggle<4>(this, FuncControls<N>::OffsetRangeSwitch + channel, "Offset" + channelName + " range",
                      {"0–5 V", "±5 V", "0–10 V", "±10 V"}, 1);
      configToggle<4>(this, FuncControls<N>::MultiplierRangeSwitch + channel, "Multiplier" + channelName + " range",
                      {"0–1", "±1", "0–2", "±2"}, 2);

      auto const operandKnobParamQuantity
          = dynamic_cast<FuncOperandKnobParamQuantity *>(paramQuantities[FuncControls<N>::AmountKnob + channel]);

      operandKnobParamQuantity->configure(params[FuncControls<N>::OperationSwitch + channel],
                                          params[FuncControls<N>::MultiplierRangeSwitch + channel],
                                          params[FuncControls<N>::OffsetRangeSwitch + channel]);
    }

  private:
    Func<N> channel{inputs, outputs, params};
  };
} // namespace func
} // namespace dhe
