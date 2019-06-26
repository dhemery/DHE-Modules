#pragma once
#include <engine/Module.hpp>

namespace dhe {

class Flogic : public rack::engine::Module {
public:
  Flogic();
  void process(const ProcessArgs &args) override;

  void setOutputs(int outputId, int negatedOutputId, float voltage) {
    outputs[outputId].setVoltage(voltage);
    outputs[negatedOutputId].setVoltage(10.F - voltage);
  }

  enum ParameterIds { NotAButton, NotBButton, ParameterCount };

  enum InputIds { AInput, BInput, InputCount };

  enum OutputIds {
    AndOutput,
    NandOutput,
    OrOutput,
    NorOutput,
    NotAOutput,
    NotBOutput,
    XorOutput,
    XnorOutput,
    PAndOutput,
    PNandOutput,
    POrOutput,
    PNorOutput,
    AImpliesBOutput,
    ANotImpliesBOutput,
    BImpliesAOutput,
    BNotImpliesAOutput,
    OutputCount
  };
};

} // namespace dhe
