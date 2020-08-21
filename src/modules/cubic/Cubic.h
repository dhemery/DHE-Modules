#pragma once

#include "CubicControls.h"
#include "components/range.h"
#include "config/common-config.h"
#include "controls/common-inputs.h"

#include <engine/Module.hpp>

namespace dhe {

namespace cubic {

static inline auto coefficientRange() -> Range const & {
  static auto constexpr coefficientRange = Range{-2.F, 2.F};
  return coefficientRange;
}

class Cubic : public rack::engine::Module {
  using Controls = CubicControls;

public:
  explicit Cubic() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);
    config_knob(this, Controls::ACoefficientKnob, "x³ coefficient", "",
                coefficientRange());
    config_knob(this, Controls::BCoefficientKnob, "x² coefficient", "",
                coefficientRange());
    config_knob(this, Controls::CCoefficientKnob, "x¹ coefficient", "",
                coefficientRange());
    config_knob(this, Controls::DCoefficientKnob, "x⁰ coefficient", "",
                coefficientRange());
    config_gain(this, Controls::InputGainKnob, "Input gain");
    config_gain(this, Controls::OutputGainKnob, "Output gain");
  }

  void process(ProcessArgs const & /*args*/) override {
    auto a =
        coefficient(Controls::ACoefficientKnob, Controls::ACoefficientCvInput);
    auto b =
        coefficient(Controls::BCoefficientKnob, Controls::BCoefficientCvInput);
    auto c =
        coefficient(Controls::CCoefficientKnob, Controls::CCoefficientCvInput);
    auto d =
        coefficient(Controls::DCoefficientKnob, Controls::DCoefficientCvInput);
    auto inputGain = gain(Controls::InputGainKnob, Controls::InputGainCvInput);
    auto outputGain =
        gain(Controls::OutputGainKnob, Controls::OutputGainCvInput);

    auto x = inputGain * mainIn() * 0.2F;
    auto x2 = x * x;
    auto x3 = x2 * x;
    auto y = a * x3 + b * x2 + c * x + d;
    auto outputVoltage = outputGain * y * 5.F;
    sendMainOut(outputVoltage);
  }

private:
  auto coefficient(int knobParam, int cvParam) const -> float {
    return coefficientRange().scale(
        rotation(params[knobParam], inputs[cvParam]));
  }

  auto gain(int knobParam, int cvInput) const -> float {
    return gain_range.scale(rotation(params[knobParam], inputs[cvInput]));
  }

  auto mainIn() const -> float {
    return voltage_at(inputs[Controls::CubicInput]);
  }

  void sendMainOut(float voltage) {
    outputs[Controls::CubicOutput].setVoltage(voltage);
  }
};
} // namespace cubic
} // namespace dhe
