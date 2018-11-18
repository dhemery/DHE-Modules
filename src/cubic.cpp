#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

namespace DHE {

struct Cubic : Module {
  static constexpr Range GAIN_RANGE{0.f, 2.f};
  static constexpr Range COEFFICIENT_RANGE{-2.f, 2.f};

  const std::function<float()> x0_knob = knob(X0_KNOB, X0_CV);
  const std::function<float()> x1_knob = knob(X1_KNOB, X1_CV);
  const std::function<float()> x2_knob = knob(X2_KNOB, X2_CV);
  const std::function<float()> x3_knob = knob(X3_KNOB, X3_CV);
  const std::function<float()> input_gain_knob = knob(INPUT_GAIN_KNOB, INPUT_GAIN_CV);
  const std::function<float()> output_gain_knob =
      knob(OUTPUT_GAIN_KNOB, OUTPUT_GAIN_CV);

  Cubic() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  float a() const { return COEFFICIENT_RANGE.scale(x3_knob()); }
  float b() const { return COEFFICIENT_RANGE.scale(x2_knob()); }
  float c() const { return COEFFICIENT_RANGE.scale(x1_knob()); }
  float d() const { return COEFFICIENT_RANGE.scale(x0_knob()); }
  float input_gain() const { return GAIN_RANGE.scale(input_gain_knob()); }
  float output_gain() const { return GAIN_RANGE.scale(output_gain_knob()); }

  void step() override {
    auto x = input_gain()*input(IN)/5.f;
    auto x2 = x*x;
    auto x3 = x2*x;

    auto y = output_gain()*(a()*x3 + b()*x2 + c()*x + d());

    outputs[OUT].value = 5.f*y;
  }

  enum ParameterIds {
    X3_KNOB,
    X2_KNOB,
    X1_KNOB,
    X0_KNOB,
    INPUT_GAIN_KNOB,
    OUTPUT_GAIN_KNOB,
    PARAMETER_COUNT
  };
  enum InputIds {
    IN,
    X3_CV,
    X2_CV,
    X1_CV,
    X0_CV,
    INPUT_GAIN_CV,
    OUTPUT_GAIN_CV,
    INPUT_COUNT
  };
  enum OutputIds { OUT, OUTPUT_COUNT };
};

struct CubicWidget : public ModuleWidget {
  explicit CubicWidget(rack::Module *module) : ModuleWidget(module, 5, "cubic") {
    auto widget_right_edge = width();

    auto left_x = width()/4.f + 0.333333f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 20.f;
    auto row_spacing = 15.f;

    auto row = 0;
    install_input(Cubic::X3_CV, {left_x, top_row_y + row*row_spacing});
    install_knob("small", Cubic::X3_KNOB,
                 {right_x, top_row_y + row*row_spacing});

    row++;
    install_input(Cubic::X2_CV, {left_x, top_row_y + row*row_spacing});
    install_knob("small", Cubic::X2_KNOB,
                 {right_x, top_row_y + row*row_spacing});

    row++;
    install_input(Cubic::X1_CV, {left_x, top_row_y + row*row_spacing});
    install_knob("small", Cubic::X1_KNOB,
                 {right_x, top_row_y + row*row_spacing});

    row++;
    install_input(Cubic::X0_CV, {left_x, top_row_y + row*row_spacing});
    install_knob("small", Cubic::X0_KNOB,
                 {right_x, top_row_y + row*row_spacing});

    top_row_y = 82.f;
    row = 0;
    install_knob("small", Cubic::INPUT_GAIN_KNOB,
                 {left_x, top_row_y + row*row_spacing});
    install_knob("small", Cubic::OUTPUT_GAIN_KNOB,
                 {right_x, top_row_y + row*row_spacing});

    row++;
    install_input(Cubic::INPUT_GAIN_CV,
                  {left_x, top_row_y + row*row_spacing});
    install_input(Cubic::OUTPUT_GAIN_CV,
                  {right_x, top_row_y + row*row_spacing});

    row++;
    install_input(Cubic::IN, {left_x, top_row_y + row*row_spacing});
    install_output(Cubic::OUT, {right_x, top_row_y + row*row_spacing});
  }
};
} // namespace DHE
rack::Model *modelCubic = rack::Model::create<DHE::Cubic, DHE::CubicWidget>(
    "DHE-Modules", "Cubic", "Cubic", rack::FUNCTION_GENERATOR_TAG);
