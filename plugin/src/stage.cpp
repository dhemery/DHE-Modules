#include <engine.hpp>

#include "display/controls.h"
#include "display/panel.h"
#include "stage/stage-module.h"
#include "util/duration.h"
#include "util/signal.h"

namespace DHE {

class Stage : public StageModule {
public:
  Stage() : StageModule{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT}{}

  auto duration() const -> float override {
    auto rotation = params[DURATION_KNOB].value;
    return DHE::duration(rotation);
  }

  auto defer_gate_in() const -> bool override { return inputs[DEFER_GATE_IN].value > 0.1; }

  auto stage_gate_in() const -> bool override { return inputs[STAGE_TRIGGER_IN].value > 0.1; }


  void send_phase(float phase) override {
    send_out(scale(taper(phase), held_voltage(), level()));
  }

  void set_active(bool active) override {
    outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
  }

  auto envelope_in() const -> float override { return inputs[ENVELOPE_IN].value; }

  auto level() const -> float override {
    auto rotation = params[LEVEL_KNOB].value;
    return Signal::unipolar_range.scale(rotation);
  }

  void set_eoc(bool eoc) override { outputs[EOC_OUT].value = eoc ? 10.f : 0.f; }

  void send_out(float voltage) override { outputs[MAIN_OUT].value = voltage; }

  enum ParameterIIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { ENVELOPE_IN, STAGE_TRIGGER_IN, DEFER_GATE_IN, INPUT_COUNT };

  enum OutputIds { MAIN_OUT, EOC_OUT, ACTIVE_OUT, OUTPUT_COUNT };

private:
  auto curvature() const -> float {
    auto rotation = params[CURVE_KNOB].value;
    return Sigmoid::curvature(rotation);
  }

  auto taper(float phase) const -> float {
    return Sigmoid::j_taper(phase, curvature());
  }
};

class StagePanel : public Panel<StagePanel> {
public:
  explicit StagePanel(Stage *module) : Panel{module, hp} {
    auto widget_right_edge = width();

    auto column_1 = width() / 4.f + 0.333333f;
    auto column_2 = widget_right_edge / 2.f;
    auto column_3 = widget_right_edge - column_1;

    auto y = 25.f;
    auto dy = 18.5f;

    install(column_2, y, knob<LargeKnob>(Stage::LEVEL_KNOB));

    y += dy;
    install(column_2, y, knob<LargeKnob>(Stage::CURVE_KNOB));

    y += dy;
    install(column_2, y, knob<LargeKnob>(Stage::DURATION_KNOB));

    y = 82.f;
    dy = 15.f;

    install(column_1, y, input(Stage::DEFER_GATE_IN));
    install(column_3, y, output(Stage::ACTIVE_OUT));

    y += dy;
    install(column_1, y, input(Stage::STAGE_TRIGGER_IN));
    install(column_3, y, output(Stage::EOC_OUT));

    y += dy;
    install(column_1, y, input(Stage::ENVELOPE_IN));
    install(column_3, y, output(Stage::MAIN_OUT));
  }

  static constexpr auto module_slug = "stage";

private:
  static constexpr auto hp = 5;
};
} // namespace DHE
rack::Model *modelStage = rack::Model::create<DHE::Stage, DHE::StagePanel>(
    "DHE-Modules", "Stage", "Stage", rack::ENVELOPE_GENERATOR_TAG);
