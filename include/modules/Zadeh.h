#pragma once
#include <engine/Module.hpp>

namespace dhe {

class Zadeh : public rack::engine::Module {
public:
  Zadeh();
  void process(const ProcessArgs &args) override;

  void setOutputs(int outputId, int negatedOutputId, float voltage) {
    outputs[outputId].setVoltage(voltage);
    outputs[negatedOutputId].setVoltage(10.F - voltage);
  }

  enum ParameterIds { NotAButtons, NotBButtons = NotAButtons + 2, ParameterCount = NotBButtons + 2 };

  enum InputIds { AInputs, BInputs = AInputs + 2, InputCount = BInputs + 2 };

  enum OutputIds {
    AndOutputs,
    NandOutputs = AndOutputs + 2,
    OrOutputs = NandOutputs + 2,
    NorOutputs = OrOutputs + 2,
    XorOutputs = NorOutputs + 2,
    XnorOutputs = XorOutputs + 2,
    AImpliesBOutputs = XnorOutputs + 2,
    ANotImpliesBOutputs = AImpliesBOutputs + 2,
    BImpliesAOutputs = ANotImpliesBOutputs + 2,
    BNotImpliesAOutputs = BImpliesAOutputs + 2,
    OutputCount = BNotImpliesAOutputs + 2
  };
};

} // namespace dhe
