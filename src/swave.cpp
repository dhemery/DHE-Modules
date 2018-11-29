#include "dhe-modules.h"
#include "module-widget.h"

#include "controls/knob.h"
#include "util/range.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

struct Swave : rack::Module {
  enum ParameterIds { CURVE_KNOB, SHAPE_SWITCH, PARAMETER_COUNT };
  enum InputIds { CURVE_CV, MAIN_IN, INPUT_COUNT };
  enum OutputIds { MAIN_OUT, OUTPUT_COUNT };

  Swave() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  auto curve() const -> float {
    auto rotation = params[CURVE_KNOB].value;
    auto cv = inputs[CURVE_CV].value;
    return modulated(rotation, cv);
  }

  void send_signal(float voltage) { outputs[MAIN_OUT].value = voltage; }

  auto shape() const -> float { return params[SHAPE_SWITCH].value; }

  auto signal_in() const -> float { return inputs[MAIN_IN].value; }

  void step() override {
    auto phase = Signal::bipolar_range.normalize(signal_in());
    auto shaped = Sigmoid::taper(phase, curve(), shape());
    auto out_voltage = Signal::bipolar_range.scale(shaped);
    send_signal(out_voltage);
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
