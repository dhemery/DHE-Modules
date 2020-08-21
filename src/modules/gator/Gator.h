#pragma once

#include "GatorControls.h"
#include "controls/common-inputs.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {

namespace gator {

class Gator : public rack::engine::Module {
  using Controls = GatorControls;

public:
  Gator() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);
    for (auto i = 0; i < Controls::inputCount; i++) {
      configParam(Controls::NegateButtons + i, 0.F, 1.F, 0.F,
                  "Negate input " + std::to_string(i + 1));
    }
  }

  void process(ProcessArgs const & /*ignored*/) override {
    auto connectedCount = 0;
    auto highCount = 0;

    for (auto i = 0; i < Controls::inputCount; i++) {
      if (inputs[i].isConnected()) {
        connectedCount++;
        if (is_high(inputs[(Controls::Inputs + i)]) !=
            is_pressed(params[Controls::NegateButtons + i])) {
          highCount++;
        }
      }
    }

    if (connectedCount == 0) {
      setAllOutputsFalse();
    } else {
      setOutputs(Controls::AndOutput, Controls::NandOutput,
                 highCount == connectedCount);
      setOutputs(Controls::OrOutput, Controls::NorOutput, highCount > 0);
      setOutputs(Controls::OddOutput, Controls::EvenOutput,
                 (highCount & 1) > 0);
      setOutputs(Controls::XorOutput, Controls::XnorOutput, highCount == 1);
    }
  }

private:
  void setAllOutputsFalse() {
    for (auto i = 0; i < Controls::OutputCount; i++) {
      outputs[i].setVoltage(0.F);
    }
  }

  void setOutputs(int outputId, int negatedOutputId, bool value) {
    auto const voltage = value ? 10.F : 0.F;
    outputs[outputId].setVoltage(voltage);
    outputs[negatedOutputId].setVoltage(10.F - voltage);
  }
};

} // namespace gator
} // namespace dhe
