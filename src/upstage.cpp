#include <algorithm>

#include "dhe-modules.hpp"
#include "module-widget.hpp"
#include "module.hpp"

#include "util/duration.hpp"
#include "util/range.hpp"

namespace DHE {

struct Upstage : Module {
  std::function<float()> const level_knob = knob(LEVEL_KNOB, LEVEL_CV);
  std::function<bool()> const trigger_button = bool_param(TRIG_BUTTON);
  std::function<bool()> const trigger_in = bool_in(TRIG_IN);
  std::function<Range const &()> const level_range =
      signal_range(int_param(LEVEL_SWITCH));
  std::function<bool()> const wait_button = bool_param(WAIT_BUTTON);
  std::function<bool()> const wait_in = bool_in(WAIT_IN);

  Upstage() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  auto envelope_out() const -> float {
    return level_range().scale(level_knob());
  }

  auto is_waiting() const -> bool { return wait_button() || wait_in(); }

  void step() override {
    outputs[TRIG_OUT].value = trigger_out();
    outputs[ENVELOPE_OUT].value = envelope_out();
  }

  auto is_triggered() const -> bool { return trigger_button() || trigger_in(); }

  auto trigger_out() const -> float {
    return is_triggered() && !is_waiting() ? 10.f : 0.f;
  }

  enum ParameterIds {
    LEVEL_KNOB,
    TRIG_BUTTON,
    WAIT_BUTTON,
    LEVEL_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds { TRIG_IN, WAIT_IN, LEVEL_CV, INPUT_COUNT };
  enum OutputIds { TRIG_OUT, ENVELOPE_OUT, OUTPUT_COUNT };
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
    install_button("normal", Upstage::TRIG_BUTTON,
                   {right_x, top_row_y + row * row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;
    install_input(Upstage::WAIT_IN, {left_x, top_row_y + row * row_spacing});

    row++;
    install_input(Upstage::TRIG_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Upstage::TRIG_OUT, {right_x, top_row_y + row * row_spacing});

    row++;
    install_output(Upstage::ENVELOPE_OUT,
                   {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelUpstage =
    rack::Model::create<DHE::Upstage, DHE::UpstageWidget>(
        "DHE-Modules", "Upstage", "Upstage", rack::ENVELOPE_GENERATOR_TAG);
