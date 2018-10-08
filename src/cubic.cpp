#include <componentlibrary.hpp>

#include "dhe-modules.hpp"
#include "module.hpp"
#include "module-widget.hpp"

namespace DHE {

struct Cubic : Module {
  Cubic() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  }

  static const Range &gain_range() {
    static constexpr auto gain_range = Range{0.f, 2.f};
    return gain_range;
  }

  static const Range &coefficient_range() {
    static constexpr auto coefficient_range = Range{-2.f, 2.f};
    return coefficient_range;
  }

  float gain(int knob, int cv) const {
    return gain_range().scale(modulated(knob, cv));
  }

  float coefficient(int knob, int cv) const {
    return coefficient_range().scale(modulated(knob, cv));
  }

  float a() const { return coefficient(X3_KNOB, X3_CV); }
  float b() const { return coefficient(X2_KNOB, X2_CV); }
  float c() const { return coefficient(X1_KNOB, X1_CV); }
  float d() const { return coefficient(X0_KNOB, X0_CV); }
  float input_gain() const { return gain(INPUT_GAIN_KNOB, INPUT_GAIN_CV); }
  float output_gain() const { return gain(OUTPUT_GAIN_KNOB, OUTPUT_GAIN_CV); }

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
  enum OutputIds {
    OUT,
    OUTPUT_COUNT
  };
};

struct CubicKnobsmall : rack::RoundKnob {
  CubicKnobsmall() {
    setSVG(rack::SVG::load(rack::assetPlugin(plugin, "res/cubic/knob-small.svg")));
    shadow->opacity = 0.f;
  }
};

struct CubicPort : rack::SVGPort {
  CubicPort() {
    background->svg = rack::SVG::load(assetPlugin(plugin, "res/cubic/port.svg"));
    background->wrap();
    box.size = background->box.size;
  }
};

struct CubicWidget : public ModuleWidget {
  CubicWidget(rack::Module *module) : ModuleWidget(module, 5, "res/cubic/panel.svg") {
    auto widget_right_edge = width();

    auto left_x = width()/4.f + 0.333333f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 20.f;
    auto row_spacing = 15.f;

    auto row = 0;
    install_input<CubicPort>(Cubic::X3_CV, {left_x, top_row_y + row*row_spacing});
    install_knob<CubicKnobsmall>(Cubic::X3_KNOB, {right_x, top_row_y + row*row_spacing});

    row++;
    install_input<CubicPort>(Cubic::X2_CV, {left_x, top_row_y + row*row_spacing});
    install_knob<CubicKnobsmall>(Cubic::X2_KNOB, {right_x, top_row_y + row*row_spacing});

    row++;
    install_input<CubicPort>(Cubic::X1_CV, {left_x, top_row_y + row*row_spacing});
    install_knob<CubicKnobsmall>(Cubic::X1_KNOB, {right_x, top_row_y + row*row_spacing});

    row++;
    install_input<CubicPort>(Cubic::X0_CV, {left_x, top_row_y + row*row_spacing});
    install_knob<CubicKnobsmall>(Cubic::X0_KNOB, {right_x, top_row_y + row*row_spacing});

    top_row_y = 82.f;
    row = 0;
    install_knob<CubicKnobsmall>(Cubic::INPUT_GAIN_KNOB, {left_x, top_row_y + row*row_spacing});
    install_knob<CubicKnobsmall>(Cubic::OUTPUT_GAIN_KNOB, {right_x, top_row_y + row*row_spacing});

    row++;
    install_input<CubicPort>(Cubic::INPUT_GAIN_CV, {left_x, top_row_y + row*row_spacing});
    install_input<CubicPort>(Cubic::OUTPUT_GAIN_CV, {right_x, top_row_y + row*row_spacing});

    row++;
    install_input<CubicPort>(Cubic::IN, {left_x, top_row_y + row*row_spacing});
    install_output<CubicPort>(Cubic::OUT, {right_x, top_row_y + row*row_spacing});
  }
};
}
rack::Model *modelCubic = rack::Model::create<DHE::Cubic, DHE::CubicWidget>("DHE-Modules", "Cubic", "Cubic", rack::FUNCTION_GENERATOR_TAG);
