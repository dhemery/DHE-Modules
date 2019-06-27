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
    ZAndOutput,
    ZNandOutput,
    ZOrOutput,
    ZNorOutput,
    ZXorOutput,
    ZXnorOutput,
    ZImplicationOutput,
    ZNonimplicationOutput,
    ZConverseImplicationOutput,
    ZConverseNonimplicationOutput,
    HAndOutput,
    HNandOutput,
    HOrOutput,
    HNorOutput,
    HXorOutput,
    HXnorOutput,
    HImplicationOutput,
    HNonimplicationOutput,
    HConverseImplicationOutput,
    HConverseNonimplicationOutput,
    OutputCount
  };
};

} // namespace dhe
