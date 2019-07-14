#pragma once
#include <engine/Module.hpp>

namespace dhe {

class Gator : public rack::engine::Module {
public:
  Gator();
  void process(const ProcessArgs &args) override;

  void setAllOutputsFalse() {
    for (int i = 0; i < OutputCount; i++) {
      outputs[i].setVoltage(0.F);
    }
  }

  void setOutputs(int outputId, int negatedOutputId, bool value) {
    auto const voltage = value ? 10.F : 0.F;
    outputs[outputId].setVoltage(voltage);
    outputs[negatedOutputId].setVoltage(10.F - voltage);
  }

  static auto constexpr inputCount = 16;

  enum ParameterIds { ENUMS(NegateButtons, inputCount), ParameterCount };

  enum InputIds { ENUMS(Inputs, inputCount), InputCount };

  enum OutputIds {
    AndOutput,
    NandOutput,
    OrOutput,
    NorOutput,
    EvenOutput,
    OddOutput,
    XorOutput,
    XnorOutput,
    OutputCount
  };
};

} // namespace dhe
