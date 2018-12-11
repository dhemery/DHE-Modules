#include "dhe-modules.h"
#include "module-widget.h"

#include "display/controls.h"
#include "util/knob.h"
#include "util/signal.h"

namespace DHE {

class Cubic : public rack::Module {
public:
  Cubic() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto a = coefficient(A_KNOB, A_CV);
    auto b = coefficient(B_KNOB, B_CV);
    auto c = coefficient(C_KNOB, C_CV);
    auto d = coefficient(D_KNOB, D_CV);
    auto input_gain = gain(INPUT_GAIN_KNOB, INPUT_GAIN_CV);
    auto output_gain = gain(OUTPUT_GAIN_KNOB, OUTPUT_GAIN_CV);

    auto x = input_gain * main_in() * 0.2f;
    auto x2 = x * x;
    auto x3 = x2 * x;
    auto y = a * x3 + b * x2 + c * x + d;
    auto output_voltage = output_gain * y * 5.f;
    send_main_out(output_voltage);
  }
  enum ParameterIds {
    A_KNOB,
    B_KNOB,
    C_KNOB,
    D_KNOB,
    INPUT_GAIN_KNOB,
    OUTPUT_GAIN_KNOB,
    PARAMETER_COUNT
  };

  enum InputIds {
    MAIN_IN,
    A_CV,
    B_CV,
    C_CV,
    D_CV,
    INPUT_GAIN_CV,
    OUTPUT_GAIN_CV,
    INPUT_COUNT
  };

  enum OutputIds { MAIN_OUT, OUTPUT_COUNT };

private:
  auto coefficient(ParameterIds knob_param, InputIds cv_param) const -> float {
    static auto constexpr coefficient_range = Range{-2.0f, 2.0f};
    return coefficient_range.scale(modulated(knob_param, cv_param));
  }

  auto gain(const ParameterIds knob_param, const InputIds cv_input) const
      -> float {
    return Knob::gain_multiplier(modulated(knob_param, cv_input));
  }

  auto main_in() const -> float { return inputs[MAIN_IN].value; }

  auto modulated(ParameterIds knob_param, InputIds cv_input) const -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    return Knob::modulated(rotation, cv);
  }

  void send_main_out(float voltage) { outputs[MAIN_OUT].value = voltage; }
};

class CubicKnob : public SmallKnob {
public:
  CubicKnob() : SmallKnob("cubic") {}
};

struct CubicWidget : public ModuleWidget<CubicWidget, Cubic> {
  static constexpr auto resource_name = "cubic";

  explicit CubicWidget(Cubic *module)
      : ModuleWidget(module, 5) {
    auto widget_right_edge = width();

    auto column_1 = width() / 4.f + 0.333333f;
    auto column_2 = widget_right_edge - column_1;

    auto y = 20.f;
    auto dy = 15.f;

    install(column_1, y, input_jack(Cubic::A_CV));
    install(column_2, y, knob<CubicKnob>(Cubic::A_KNOB));

    y += dy;
    install(column_1, y, input_jack(Cubic::B_CV));
    install(column_2, y, knob<CubicKnob>(Cubic::B_KNOB));

    y += dy;
    install(column_1, y, input_jack(Cubic::C_CV));
    install(column_2, y, knob<CubicKnob>(Cubic::C_KNOB));

    y += dy;
    install(column_1, y, input_jack(Cubic::D_CV));
    install(column_2, y, knob<CubicKnob>(Cubic::D_KNOB));

    y = 82.f;
    install(column_1, y, knob<CubicKnob>(Cubic::INPUT_GAIN_KNOB));
    install(column_2, y, knob<CubicKnob>(Cubic::OUTPUT_GAIN_KNOB));

    y += dy;
    install(column_1, y, input_jack(Cubic::MAIN_IN));
    install(column_2, y, output_jack(Cubic::MAIN_OUT));

  }
};
} // namespace DHE
rack::Model *modelCubic = rack::Model::create<DHE::Cubic, DHE::CubicWidget>(
    "DHE-Modules", "Cubic", "Cubic", rack::FUNCTION_GENERATOR_TAG);
