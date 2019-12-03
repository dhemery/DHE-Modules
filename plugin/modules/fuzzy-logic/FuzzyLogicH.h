#pragma once
#include "FuzzyLogicControls.h"
#include "config/LevelConfig.h"
#include "controls/CommonInputs.h"

#include <engine/Module.hpp>

namespace dhe {

namespace fuzzy_logic {

  class FuzzyLogicH : public rack::engine::Module {
    using Controls = FuzzyLogicControls;

  public:
    FuzzyLogicH() {
      config(Controls::ParameterCount, Controls::InputCount, Controls::OutputCount);
      configParam(Controls::NotAButtons + 0, 0.F, 1.F, 0.F, "Negate A");
      configParam(Controls::NotBButtons + 0, 0.F, 1.F, 0.F, "Negate B");
      configParam(Controls::NotAButtons + 1, 0.F, 1.F, 0.F, "Negate C");
      configParam(Controls::NotBButtons + 1, 0.F, 1.F, 0.F, "Negate D");
      configLevelRangeSwitch(this, Controls::LevelRangeSwitch, "Level Range");
    }

    void process(ProcessArgs const & /*ignored*/) {
      auto const voltageOffset = positionOf(params[Controls::LevelRangeSwitch]) == 1 ? 0.F : 5.F;
      for (auto i = 0; i < 2; i++) {
        auto const aInput = inputs[Controls::AInputs + i].getVoltage() + voltageOffset;
        auto const bInput = inputs[Controls::BInputs + i].getVoltage() + voltageOffset;
        auto const a = isPressed(params[(Controls::NotAButtons + i)]) ? 10.F - aInput : aInput;
        auto const notA = 10.F - a;
        auto const b = isPressed(params[(Controls::NotBButtons + i)]) ? 10.F - bInput : bInput;
        auto const notB = 10.F - b;

        auto const aAndB = a * b * 0.1F;
        auto const aOrB = a + b - aAndB;
        auto const aXorB = aOrB - aAndB;
        auto const aImpliesB = notA + aAndB;
        auto const bImpliesA = notB + aAndB;

        setOutputs(Controls::AndOutputs + i, Controls::NandOutputs + i, aAndB, voltageOffset);
        setOutputs(Controls::OrOutputs + i, Controls::NorOutputs + i, aOrB, voltageOffset);
        setOutputs(Controls::XorOutputs + i, Controls::XnorOutputs + i, aXorB, voltageOffset);
        setOutputs(Controls::ImplicationOutputs + i, Controls::NonimplicationOutputs + i, aImpliesB, voltageOffset);
        setOutputs(Controls::ConverseImplicationOutputs + i, Controls::ConverseNonimplicationOutputs + i, bImpliesA,
                   voltageOffset);
      }
    }

    void setOutputs(int outputId, int negatedOutputId, float voltage, float offset) {
      outputs[outputId].setVoltage(voltage - offset);
      outputs[negatedOutputId].setVoltage(10.F - voltage - offset);
    }
  };
} // namespace fuzzy_logic
} // namespace dhe
