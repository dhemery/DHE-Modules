#include <utility>

#include "dhe-modules.h"
#include "module-widget.h"

#include "controls/knob.h"

namespace DHE {

struct ScaledKnob {
  const Knob knob;
  const Range range;

  ScaledKnob(const rack::Module *module, int knob, int cv, const Range &range)
      : knob{Knob::with_cv(module, knob, cv)}, range{range} {}

  auto operator()() const -> float { return range.scale(knob()); }
};

struct Cubic : rack::Module {
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

  static Range constexpr coefficient_range{-2.0f, 2.0f};
  static Range constexpr gain_range{0.f, 2.0f};

  ScaledKnob const a{this, A_KNOB, A_CV, coefficient_range};
  ScaledKnob const b{this, B_KNOB, B_CV, coefficient_range};
  ScaledKnob const c{this, C_KNOB, C_CV, coefficient_range};
  ScaledKnob const d{this, D_KNOB, D_CV, coefficient_range};

  ScaledKnob const input_gain{this, INPUT_GAIN_KNOB, INPUT_GAIN_CV, gain_range};
  ScaledKnob const output_gain{this, OUTPUT_GAIN_KNOB, OUTPUT_GAIN_CV,
                               gain_range};

  Cubic() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto x = input_gain() * main_in() * 0.2f;
    auto x2 = x * x;
    auto x3 = x2 * x;
    auto y = a() * x3 + b() * x2 + c() * x + d();
    auto output_voltage = output_gain() * y * 5.f;
    send_main_out(output_voltage);
  }

  auto main_in() const -> float { return inputs[MAIN_IN].value; }

  void send_main_out(float voltage) { outputs[MAIN_OUT].value = voltage; }
};

struct CubicWidget : public ModuleWidget {
  explicit CubicWidget(rack::Module *module)
      : ModuleWidget(module, 5, "cubic") {
    auto widget_right_edge = width();

    auto left_x = width() / 4.f + 0.333333f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 20.f;
    auto row_spacing = 15.f;

    auto row = 0;
    install_input(Cubic::A_CV, {left_x, top_row_y + row * row_spacing});
    install_knob("small", Cubic::A_KNOB,
                 {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Cubic::B_CV, {left_x, top_row_y + row * row_spacing});
    install_knob("small", Cubic::B_KNOB,
                 {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Cubic::C_CV, {left_x, top_row_y + row * row_spacing});
    install_knob("small", Cubic::C_KNOB,
                 {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Cubic::D_CV, {left_x, top_row_y + row * row_spacing});
    install_knob("small", Cubic::D_KNOB,
                 {right_x, top_row_y + row * row_spacing});

    top_row_y = 82.f;
    row = 0;
    install_knob("small", Cubic::INPUT_GAIN_KNOB,
                 {left_x, top_row_y + row * row_spacing});
    install_knob("small", Cubic::OUTPUT_GAIN_KNOB,
                 {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Cubic::INPUT_GAIN_CV,
                  {left_x, top_row_y + row * row_spacing});
    install_input(Cubic::OUTPUT_GAIN_CV,
                  {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Cubic::MAIN_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Cubic::MAIN_OUT, {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelCubic = rack::Model::create<DHE::Cubic, DHE::CubicWidget>(
    "DHE-Modules", "Cubic", "Cubic", rack::FUNCTION_GENERATOR_TAG);
