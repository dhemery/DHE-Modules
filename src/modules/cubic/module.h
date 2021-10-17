#pragma once

#include "control-ids.h"

#include "components/range.h"
#include "params/common-config.h"
#include "params/float-params.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

namespace dhe {

namespace cubic {

struct Coefficient : public LinearFloat<Coefficient> {
  static auto constexpr display_range = Range{-2.F, 2.F};
  static auto constexpr default_display_value = 0.F;
  static auto constexpr unit = "";

  static inline auto value(float rotation) -> float {
    return display_range.scale(rotation);
  }
};

class Module : public rack::engine::Module {
  using Controls = CubicControls;

public:
  explicit Module() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    Coefficient::config(this, Controls::ACoefficientKnob,
                        "X cubed coefficient");
    configInput(Controls::ACoefficientCvInput, "X cubed coefficient CV");

    Coefficient::config(this, Controls::BCoefficientKnob,
                        "X squared coefficient");
    configInput(Controls::BCoefficientCvInput, "X squared coefficient CV");

    Coefficient::config(this, Controls::CCoefficientKnob, "X coefficient", 1.F);
    configInput(Controls::CCoefficientCvInput, "X coefficient CV");

    Coefficient::config(this, Controls::DCoefficientKnob,
                        "Constant coefficient");
    configInput(Controls::DCoefficientCvInput, "Constant coefficient CV");

    Gain::config(this, Controls::InputGainKnob, "InPort gain");
    configInput(Controls::InputGainCvInput, "InPort gain CV");

    Gain::config(this, Controls::OutputGainKnob, "OutPort gain");
    configInput(Controls::OutputGainCvInput, "OutPort gain CV");

    configInput(Controls::CubicInput, "Module");
    configOutput(Controls::CubicOutput, "Module");
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
    return Coefficient::value(rotation(params[knob_param], inputs[cv_param]));
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
