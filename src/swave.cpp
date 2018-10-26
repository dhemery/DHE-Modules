#include <algorithm>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/controls.hpp"
#include "util/range.hpp"

namespace DHE {

struct Swave : Module {
  Swave() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  bool is_s_taper() const { return param(SHAPE_SWITCH) > 0.5f; }

  void step() override {
    outputs[SWAVE_OUT].value = to_signal(taper(to_phase(swave_in())));
  }

  float swave_in() const { return input(SWAVE_IN); }

  float taper(float phase) const {
    auto rotation = modulated(CURVE_KNOB, CURVE_CV);
    return is_s_taper() ? Taper::s(phase, rotation) : Taper::j(phase, rotation);
  }

  float to_signal(float phase) const {
    return BIPOLAR_SIGNAL_RANGE.scale(phase);
  }

  float to_phase(float signal) const {
    return BIPOLAR_SIGNAL_RANGE.normalize(signal);
  }

  enum ParameterIds { CURVE_KNOB, SHAPE_SWITCH, PARAMETER_COUNT };
  enum InputIds { CURVE_CV, SWAVE_IN, INPUT_COUNT };
  enum OutputIds { SWAVE_OUT, OUTPUT_COUNT };
};

struct SwaveWidget : public ModuleWidget {
  SwaveWidget(rack::Module *module) : ModuleWidget(module, 4, "swave") {
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
    install_input(Swave::SWAVE_IN, {center_x, top_row_y + row * row_spacing});

    row++;
    install_output(Swave::SWAVE_OUT, {center_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelSwave = rack::Model::create<DHE::Swave, DHE::SwaveWidget>(
    "DHE-Modules", "Swave", "Swave", rack::WAVESHAPER_TAG);
