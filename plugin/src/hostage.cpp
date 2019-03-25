#include <stage/modes/generating-mode.h>
#include <stage/modes/resting-mode.h>
#include <stage/modes/deferring-mode.h>
#include <stage/modes/sustaining-mode.h>
#include "engine.hpp"

#include "display/controls.h"
#include "display/panel.h"
#include "stage/stage-state-machine.h"
#include "stage/components/stage-generator.h"
#include "util/duration.h"
#include "util/rotation.h"

namespace DHE {

class Hostage : public rack::Module {
public:
  Hostage() : rack::Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    state_machine.start(&resting_mode);
  }

  void step() override { state_machine.step(); }

  void start_deferring() {
    set_active(true);
    state_machine.enter(&deferring_mode);
  }
  void do_defer() { send_out(envelope_in()); }
  void stop_deferring() {
    if(stage_gate_in()) {
      start_generating();
    } else {
      finish_generating();
    }
  }

  void start_generating() {
    if(is_sustain_mode()) {
      start_sustaining();
    } else {
      start_holding();
    }
  }
  void start_sustaining() {
    set_active(true);
    held_voltage = envelope_in();
    state_machine.enter(&sustaining_mode);
  }
  void do_sustain() {
    send_out(held_voltage);
  }
  void finish_sustaining() {
    state_machine.generate_end_of_cycle();
    start_resting();
  }

  void start_holding() {
    set_active(true);
    held_voltage = envelope_in();
    state_machine.enter(&holding_mode);
  }
  void generate(float ignored) { do_sustain(); }
  void finish_generating() {
    state_machine.generate_end_of_cycle();
    start_resting();
  }
  void on_end_of_cycle_rise() { set_eoc(true); }
  void on_end_of_cycle_fall() { set_eoc(false); }

  void start_resting() {
    set_active(false);
    state_machine.enter(&resting_mode);
  }
  void do_rest() {
    send_out(held_voltage);
  }

  auto duration() const -> float {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    return DHE::duration(rotation, *duration_range);
  }

  auto sample_time() const -> float {
    return rack::engineGetSampleTime();
  };

  auto defer_gate_in() const -> bool { return inputs[DEFER_GATE_IN].value > 0.1f; }
  auto stage_gate_in() const -> bool { return inputs[STAGE_GATE_IN].value > 0.1f; }

  const Selector<Range const *> duration_range_selector{
      Duration::ranges, [this](Range const *range) { duration_range = range; }};

  enum InputIds {
    DEFER_GATE_IN,
    DURATION_CV,
    MAIN_IN,
    STAGE_GATE_IN,
    INPUT_COUNT
  };

  enum OutputIds { ACTIVE_OUT, MAIN_OUT, EOC_OUT, OUTPUT_COUNT };

  enum ParameterIds {
    DURATION_KNOB,
    DURATION_RANGE_SWITCH,
    HOSTAGE_MODE_SWITCH,
    PARAMETER_COUNT
  };

private:
  auto envelope_in() const -> float { return inputs[MAIN_IN].value; }

  auto level() const -> float { return held_voltage; }

  auto modulated(ParameterIds knob_param, InputIds cv_input) const -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    return Rotation::modulated(rotation, cv);
  }

  void send_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

  auto is_sustain_mode() const -> bool {
    return params[HOSTAGE_MODE_SWITCH].value > 0.5f;
  }

  void set_active(bool active) {
    outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
  }

  void set_eoc(bool eoc) { outputs[EOC_OUT].value = eoc ? 10.f : 0.f; }

  StageStateMachine<Hostage> state_machine{this};

  DeferringMode<Hostage> deferring_mode{this};
  GeneratingMode<Hostage> holding_mode{this};
  RestingMode<Hostage> resting_mode{this};
  SustainingMode<Hostage> sustaining_mode{this};

  Range const *duration_range{&Duration::medium_range};
  float held_voltage{0};
};

class HostagePanel : public Panel<HostagePanel> {
public:
  explicit HostagePanel(Hostage *module) : Panel{module, hp} {
    auto widget_right_edge = width();

    auto column_1 = width()/4.f + 0.333333f;
    auto column_2 = widget_right_edge/2.f;
    auto column_3 = widget_right_edge - column_1;

    auto y = 25.f;
    auto dy = 18.5f;

    install(column_2, y, toggle<2>(Hostage::HOSTAGE_MODE_SWITCH, 0));

    y += dy;
    install(column_1, y, input(Hostage::DURATION_CV));
    install(column_3, y,
            toggle<3>(Hostage::DURATION_RANGE_SWITCH, 1,
                      module->duration_range_selector));

    y += dy;
    install(column_2, y, knob<LargeKnob>(Hostage::DURATION_KNOB));

    y = 82.f;
    dy = 15.f;

    install(column_1, y, input(Hostage::DEFER_GATE_IN));
    install(column_3, y, output(Hostage::ACTIVE_OUT));

    y += dy;
    install(column_1, y, input(Hostage::STAGE_GATE_IN));
    install(column_3, y, output(Hostage::EOC_OUT));

    y += dy;
    install(column_1, y, input(Hostage::MAIN_IN));
    install(column_3, y, output(Hostage::MAIN_OUT));
  }

  static constexpr auto module_slug = "hostage";

private:
  static constexpr auto hp = 5;
};
} // namespace DHE
rack::Model *modelHostage =
    rack::Model::create<DHE::Hostage, DHE::HostagePanel>(
        "DHE-Modules", "Hostage", "Hostage", rack::ENVELOPE_GENERATOR_TAG);
