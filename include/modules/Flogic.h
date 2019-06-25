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

  enum ParameterIds { ANotButtons, BNotButtons = ANotButtons + 2, ParameterCount = BNotButtons + 2 };

  enum InputIds { AInputs, BInputs = AInputs + 2, InputCount = BInputs + 2 };

  enum OutputIds {
    ZOrOutputs,
    ZNorOutputs = ZOrOutputs + 2,
    ZAndOutputs = ZNorOutputs + 2,
    ZNandOutputs = ZAndOutputs + 2,
    PAndOutputs = ZNandOutputs + 2,
    PNandOutputs = PAndOutputs + 2,
    POrOutputs = PNandOutputs + 2,
    PNorOutputs = POrOutputs + 2,
    OutputCount = PNorOutputs + 2
  };
};

} // namespace dhe
