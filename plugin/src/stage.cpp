#include <engine.hpp>
#include <stage/modes/resting-mode.h>
#include <stage/modes/deferring-mode.h>
#include <stage/modes/generating-mode.h>

#include "display/controls.h"
#include "display/panel.h"
#include "stage/stage-state-machine.h"
#include "util/duration.h"
#include "util/signal.h"

namespace DHE {

class Stage : public rack::Module {
public:
  Stage() : rack::Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  auto sample_time() const -> float {
    return rack::engineGetSampleTime();
  };

  auto duration() const -> float {
    auto rotation = params[DURATION_KNOB].value;
    return DHE::duration(rotation);
  }

  auto defer_gate_in() const -> bool { return inputs[DEFER_GATE_IN].value > 0.1; }
  auto stage_gate_in() const -> bool { return inputs[STAGE_TRIGGER_IN].value > 0.1; }

  void start_deferring() {}
  void do_defer() {}
  void stop_deferring() {}

  void start_generating() {}
  void do_generate() {}
  void stop_generating() {}

  void do_rest() {}


  void on_eoc_start() {
    set_eoc(true);
  }

  void on_eoc_end() {
    set_eoc(false);
  }

  void send_phase(float phase) {
    send_out(scale(taper(phase), held_voltage, level()));
  }

  void set_active(bool active) {
    outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
  }

  auto envelope_in() const -> float { return inputs[ENVELOPE_IN].value; }

  auto level() const -> float {
    auto rotation = params[LEVEL_KNOB].value;
    return Signal::unipolar_range.scale(rotation);
  }

  void set_eoc(bool eoc) { outputs[EOC_OUT].value = eoc ? 10.f : 0.f; }

  void send_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

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

  DeferringMode<Stage> deferring_mode{this};
  GeneratingMode<Stage> generating_mode{this};
  RestingMode<Stage> resting_mode{this};
  StageStateMachine<Stage> state_machine{this, &resting_mode};
  float held_voltage{0.f};
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
