#include "dhe-modules.h"
#include "module-widget.h"

#include "display/controls.h"
#include "util/range.h"
#include "util/rotation.h"
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
    return Rotation::modulated(rotation, cv);
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

class UpstageKnob : public LargeKnob {
public:
  UpstageKnob() : LargeKnob("upstage") {}
};

class UpstageButton : public Button {
public:
  UpstageButton() : Button{"upstage"} {}
};

class UpstageLevelRangeSwitch : public ThumbSwitch<2> {
public:
  UpstageLevelRangeSwitch() : ThumbSwitch<2>{"upstage"} {}
};

struct UpstageWidget : public ModuleWidget<UpstageWidget, Upstage> {
  static constexpr auto resource_name = "upstage";

  explicit UpstageWidget(Upstage *module) : ModuleWidget(module, 5) {
    auto widget_right_edge = width();

    auto column_1 = width() / 4.f + 0.333333333f;
    auto column_2 = widget_right_edge / 2.f;
    auto column_3 = widget_right_edge - column_1;

    auto y = 25.f;
    auto dy = 18.5f;

    install(column_2, y, knob<UpstageKnob>(Upstage::LEVEL_KNOB));

    y += dy;
    install(column_1, y, input<Jack>(Upstage::LEVEL_CV));
    install(
        column_3, y,
        thumb_switch<UpstageLevelRangeSwitch>(Upstage::LEVEL_RANGE_SWITCH, 1));

    y += dy;
    install(column_1, y, button<UpstageButton>(Upstage::WAIT_BUTTON));
    install(column_3, y, button<UpstageButton>(Upstage::TRIGGER_BUTTON));

    y = 82.f;
    dy = 15.f;

    install(column_1, y, input<Jack>(Upstage::WAIT_IN));

    y += dy;
    install(column_1, y, input<Jack>(Upstage::TRIGGER_IN));
    install(column_3, y, output<Jack>(Upstage::TRIGGER_OUT));

    y += dy;
    install(column_3, y, output<Jack>(Upstage::MAIN_OUT));
  }
};
} // namespace DHE
rack::Model *modelUpstage =
    rack::Model::create<DHE::Upstage, DHE::UpstageWidget>(
        "DHE-Modules", "Upstage", "Upstage", rack::ENVELOPE_GENERATOR_TAG);
