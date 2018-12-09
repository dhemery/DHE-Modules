#include "dhe-modules.h"
#include "module-widget.h"

#include "util/knob.h"
#include "util/range.h"
#include "util/signal.h"

namespace DHE {

class Upstage : public rack::Module {
public:
  Upstage() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto is_triggered = trigger_in() && !wait_in();
    send_trigger(is_triggered);
    send_envelope(envelope_voltage());
  }

  enum ParameterIds {
    LEVEL_KNOB,
    TRIGGER_BUTTON,
    WAIT_BUTTON,
    LEVEL_RANGE_SWITCH,
    PARAMETER_COUNT
  };

  enum InputIds { TRIGGER_IN, WAIT_IN, LEVEL_CV, INPUT_COUNT };

  enum OutputIds { TRIGGER_OUT, MAIN_OUT, OUTPUT_COUNT };

private:
  auto envelope_voltage() const -> float { return range().scale(level()); }

  auto level() const -> float { return modulated(LEVEL_KNOB, LEVEL_CV); }

  auto modulated(ParameterIds knob_param, InputIds cv_input) const -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    return Knob::modulated(rotation, cv);
  }

  auto range() const -> const Range & {
    auto is_uni = params[LEVEL_RANGE_SWITCH].value > 0.5f;
    return Signal::range(is_uni);
  }

  void send_envelope(float voltage) { outputs[MAIN_OUT].value = voltage; }

  void send_trigger(bool is_triggered) {
    outputs[TRIGGER_OUT].value = is_triggered ? 10.f : 0.f;
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

struct UpstageWidget : public ModuleWidget<UpstageWidget, Upstage> {
  static constexpr auto resource_name = "upstage";

  explicit UpstageWidget(Upstage *module)
      : ModuleWidget(module, 5) {
    auto widget_right_edge = width();

    auto left_x = width() / 4.f + 0.333333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_large_knob(Upstage::LEVEL_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    row++;
    install_input(Upstage::LEVEL_CV, {left_x, top_row_y + row * row_spacing});
    install_switch(Upstage::LEVEL_RANGE_SWITCH,
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
