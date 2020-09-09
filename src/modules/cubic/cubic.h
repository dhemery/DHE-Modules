#pragma once

#include "components/range.h"
#include "config/common-config.h"
#include "controls/common-inputs.h"
#include "cubic-controls.h"

#include <engine/Module.hpp>

namespace dhe {

namespace cubic {

static auto constexpr coefficient_range = Range{-2.F, 2.F};

class Cubic : public rack::engine::Module {
  using Controls = CubicControls;

public:
  explicit Cubic() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);
    config_knob(this, Controls::ACoefficientKnob, "x³ coefficient", "",
                coefficient_range);
    config_knob(this, Controls::BCoefficientKnob, "x² coefficient", "",
                coefficient_range);
    config_knob(this, Controls::CCoefficientKnob, "x¹ coefficient", "",
                coefficient_range);
    config_knob(this, Controls::DCoefficientKnob, "x⁰ coefficient", "",
                coefficient_range);
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
    auto input_gain = gain(Controls::InputGainKnob, Controls::InputGainCvInput);
    auto output_gain =
        gain(Controls::OutputGainKnob, Controls::OutputGainCvInput);

    auto x = input_gain * main_in() * 0.2F;
    auto x2 = x * x;
    auto x3 = x2 * x;
    auto y = a * x3 + b * x2 + c * x + d;
    auto output_voltage = output_gain * y * 5.F;
    send_main_out(output_voltage);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto coefficient(int knob_param, int cv_param) const -> float {
    return coefficient_range.scale(
        rotation(params[knob_param], inputs[cv_param]));
  }

  auto gain(int knob_param, int cv_input) const -> float {
    return gain_range.scale(rotation(params[knob_param], inputs[cv_input]));
  }

  auto main_in() const -> float {
    return voltage_at(inputs[Controls::CubicInput]);
  }

  void send_main_out(float voltage) {
    outputs[Controls::CubicOutput].setVoltage(voltage);
  }
};
} // namespace cubic
} // namespace dhe
