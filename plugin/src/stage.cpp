#include <engine.hpp>
#include <stage/components/stage-generator.h>
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
  Stage() : rack::Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    state_machine.start(&resting_mode);
  }

  void step() override { state_machine.step(); }

  void start_deferring() {
    set_active(true);
    state_machine.enter(&deferring_mode);
  }
  void do_defer() { send_out(envelope_in()); }
  void stop_deferring() {
    set_active(false);
    state_machine.enter(&resting_mode);
  }

  void start_generating() {
    set_active(true);
    held_voltage = envelope_in();
    state_machine.enter(&generating_mode);
  }
  void generate(float phase) {
    send_out(scale(taper(phase), held_voltage, level()));
  }
  void finish_generating() {
    start_resting();
  }
  void on_end_of_cycle_rise() { set_eoc(true); }
  void on_end_of_cycle_fall() { set_eoc(false); }

  void start_resting() {
    set_active(false);
    state_machine.enter(&resting_mode);
  }
  void do_rest() { send_out(level()); }

  auto defer_gate_in() const -> bool { return inputs[DEFER_GATE_IN].value > 0.1; }
  auto stage_gate_in() const -> bool { return inputs[STAGE_TRIGGER_IN].value > 0.1; }

  auto sample_time() const -> float {
    return rack::engineGetSampleTime();
  };

  auto duration() const -> float {
    auto rotation = params[DURATION_KNOB].value;
    return DHE::duration(rotation);
  }

  enum ParameterIIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { ENVELOPE_IN, STAGE_TRIGGER_IN, DEFER_GATE_IN, INPUT_COUNT };

  enum OutputIds { MAIN_OUT, EOC_OUT, ACTIVE_OUT, OUTPUT_COUNT };

private:
  auto curvature() const -> float {
    auto rotation = params[CURVE_KNOB].value;
    return Sigmoid::curvature(rotation);
  }

  auto envelope_in() const -> float { return inputs[ENVELOPE_IN].value; }

  auto level() const -> float {
    auto rotation = params[LEVEL_KNOB].value;
    return Signal::unipolar_range.scale(rotation);
  }

  void send_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

  void set_active(bool active) { outputs[ACTIVE_OUT].value = active ? 10.f : 0.f; }

  void set_eoc(bool eoc) { outputs[EOC_OUT].value = eoc ? 10.f : 0.f; }

  auto taper(float phase) const -> float {
    return Sigmoid::j_taper(phase, curvature());
  }

  StageStateMachine<Stage> state_machine{this};

  StageGenerator<Stage, StageStateMachine<Stage>> stage_generator{this, &state_machine};

  DeferringMode<Stage> deferring_mode{this};
  GeneratingMode<Stage> generating_mode{this, &stage_generator};
  RestingMode<Stage> resting_mode{this};

  float held_voltage{0.f};
};

class StagePanel : public Panel<StagePanel> {
public:
  explicit StagePanel(Stage *module) : Panel{module, hp} {
    auto widget_right_edge = width();

    auto column_1 = width()/4.f + 0.333333f;
    auto column_2 = widget_right_edge/2.f;
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
