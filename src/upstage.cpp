#include "dhe-modules.h"
#include "module-widget.h"
#include "module.h"

#include "util/duration.h"
#include "util/modulation.h"
#include "util/range.h"
#include "util/signal.h"

namespace DHE {

struct Upstage : Module {
  enum ParameterIds {
    LEVEL_KNOB,
    TRIGGER_BUTTON,
    WAIT_BUTTON,
    LEVEL_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds { TRIGGER_IN, WAIT_IN, LEVEL_CV, INPUT_COUNT };
  enum OutputIds { TRIGGER_OUT, MAIN_OUT, OUTPUT_COUNT };

  Upstage() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  auto envelope_voltage() const -> float {
    auto is_uni = params[LEVEL_SWITCH].value > 0.1f;
    auto &range = is_uni ? Signal::unipolar_range : Signal::bipolar_range;
    return range.scale(level_in());
  }

  auto level_in() const -> float {
    return Modulation::of(this, LEVEL_KNOB, LEVEL_CV);
  }

  void send_main_out(float voltage) { outputs[MAIN_OUT].value = voltage; }
  void send_trigger_out(bool is_triggered) {
    outputs[TRIGGER_OUT].value = is_triggered ? 10.f : 0.f;
  }

  void step() override {
    auto is_triggered = trigger_in() && !wait_in();
    send_trigger_out(is_triggered);
    send_main_out(envelope_voltage());
  }

  auto trigger_in() const -> bool {
    auto trigger_button = params[TRIGGER_BUTTON].value > 0.1f;
    auto trigger_input = inputs[TRIGGER_IN].value > 0.1f;
    return trigger_button || trigger_input;
  }

  auto wait_in() const -> bool {
    auto wait_button = params[WAIT_BUTTON].value > 0.1f;
    auto wait_input = inputs[WAIT_IN].value > 0.1f;
    return wait_button || wait_input;
  }
};

struct UpstageWidget : public ModuleWidget {
  explicit UpstageWidget(rack::Module *module)
      : ModuleWidget(module, 5, "upstage") {
    auto widget_right_edge = width();

    auto left_x = width() / 4.f + 0.333333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_knob("large", Upstage::LEVEL_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    row++;
    install_input(Upstage::LEVEL_CV, {left_x, top_row_y + row * row_spacing});
    install_switch(Upstage::LEVEL_SWITCH,
                   {right_x, top_row_y + row * row_spacing}, 1, 1);

    row++;
    install_button("normal", Upstage::WAIT_BUTTON,
                   {left_x, top_row_y + row * row_spacing});
    install_button("normal", Upstage::TRIGGER_BUTTON,
                   {right_x, top_row_y + row * row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;
    install_input(Upstage::WAIT_IN, {left_x, top_row_y + row * row_spacing});

    row++;
    install_input(Upstage::TRIGGER_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Upstage::TRIGGER_OUT,
                   {right_x, top_row_y + row * row_spacing});

    row++;
    install_output(Upstage::MAIN_OUT, {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelUpstage =
    rack::Model::create<DHE::Upstage, DHE::UpstageWidget>(
        "DHE-Modules", "Upstage", "Upstage", rack::ENVELOPE_GENERATOR_TAG);
