#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/range.hpp"
#include "util/sigmoid.hpp"
#include "util/signal.hpp"

namespace DHE {

struct Swave : Module {
  enum ParameterIds { CURVE_KNOB, SHAPE_SWITCH, PARAMETER_COUNT };
  enum InputIds { CURVE_CV, MAIN_IN, INPUT_COUNT };
  enum OutputIds { MAIN_OUT, OUTPUT_COUNT };

  Swave() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  auto curve_in() const -> float {
    auto const amount{modulated(CURVE_KNOB, CURVE_CV)};
    return Sigmoid::curvature(amount);
  }

  auto is_s_curve() const -> bool { return params[SHAPE_SWITCH].value > 0.1f; }

  auto main_in() const -> float { return inputs[MAIN_IN].value; }

  void send_main_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

  auto shape(float phase) const -> float {
    auto const rotation{curve_in()};
    auto const curvature{Sigmoid::curvature(rotation)};
    return is_s_curve() ? Sigmoid::s_taper(phase, curvature)
                        : Sigmoid::j_taper(phase, curvature);
  }

  void step() override {
    auto const input(main_in());
    auto const phase{Signal::bipolar_range.normalize(input)};
    auto const shaped{shape(phase)};
    auto const out_voltage{Signal::bipolar_range.scale(shaped)};
    send_main_out(out_voltage);
  }
};

struct SwaveWidget : public ModuleWidget {
  explicit SwaveWidget(rack::Module *module)
      : ModuleWidget(module, 4, "swave") {
    auto widget_right_edge = width();

    auto center_x = widget_right_edge / 2.f;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_switch(Swave::SHAPE_SWITCH,
                   {center_x, top_row_y + row * row_spacing}, 1, 1);

    row++;
    install_knob("large", Swave::CURVE_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    row++;
    install_input(Swave::CURVE_CV, {center_x, top_row_y + row * row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;

    row++;
    install_input(Swave::MAIN_IN, {center_x, top_row_y + row * row_spacing});

    row++;
    install_output(Swave::MAIN_OUT, {center_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelSwave = rack::Model::create<DHE::Swave, DHE::SwaveWidget>(
    "DHE-Modules", "Swave", "Swave", rack::WAVESHAPER_TAG);
