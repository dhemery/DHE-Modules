#pragma once
#include <engine/Module.hpp>

namespace dhe {

class Flogic : public rack::engine::Module {
public:
  Flogic();
  void process(const ProcessArgs &args) override;

  void setOutputs(int outputId, int negatedOutputId, float value) {
    outputs[outputId].setVoltage(value);
    outputs[negatedOutputId].setVoltage(10.F - value);
  }

  enum ParameterIds { NegateAButtons, NegateBButtons = NegateAButtons + 2, ParameterCount = NegateBButtons + 2 };

  enum InputIds { AInputs, BInputs = AInputs + 2, InputCount = BInputs + 2 };

  enum OutputIds {
    MaxOutputs,
    NegatedMaxOutputs = MaxOutputs + 2,
    MinOutputs = NegatedMaxOutputs + 2,
    NegatedMinOutputs = MinOutputs + 2,
    AndOutputs = NegatedMinOutputs + 2,
    NegatedAndOutputs = AndOutputs + 2,
    OrOutputs = NegatedAndOutputs + 2,
    NegatedOrOutputs = OrOutputs + 2,
    OutputCount = NegatedOrOutputs + 2
  };
};

} // namespace dhe
