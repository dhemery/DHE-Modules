#include "dhe-modules.h"
#include "module-widget.h"
#include <util/stage-components.h>

#include "util/duration.h"
#include "util/knob.h"
#include "util/phase-accumulator.h"

namespace DHE {

class Hostage : public rack::Module {
public:
  Hostage() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    mode->enter();
  }

  void step() override {
    choose_stage_type();
    defer_gate.step();
    mode->step();
    eoc_generator.step();
  }

  auto defer_gate_in() const -> bool { return inputs[DEFER_IN].value > 0.1f; }

  auto duration() const -> float {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    auto range_selection =
        static_cast<int>(params[DURATION_RANGE_SWITCH].value);
    return DHE::duration(rotation, range_selection);
  }

  void hold_input() { held_voltage = envelope_in(); }

  void on_defer_gate_rise() { enter(&deferring_mode); }

  void on_defer_gate_fall() {
    if (stage_type == SUSTAIN && sustain_gate_in()) {
      enter(&generating_mode);
    } else {
      stop_generating();
    }
  }

  void on_stage_gate_rise() { enter(&generating_mode); }

  void on_stage_generator_finish() {
    if (stage_type == HOLD) {
      stop_generating();
    }
  }

  void on_stage_gate_fall() {
    if (stage_type == SUSTAIN) {
      eoc_generator.start();
      enter(&following_mode);
    }
  }

  void send_active(bool active) {
    outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
  }

  void send_eoc(bool eoc) { outputs[EOC_OUT].value = eoc ? 10.f : 0.f; }

  void send_held() { send_out(held_voltage); }

  void send_input() { send_out(envelope_in()); }

  void send_stage() { send_held(); }

  auto sustain_gate_in() const -> bool {
    return inputs[SUSTAIN_GATE_IN].value > 0.1f;
  }

  enum InputIds {
    DEFER_IN,
    DURATION_CV,
    MAIN_IN,
    SUSTAIN_GATE_IN,
    INPUT_COUNT
  };

  enum OutputIds { ACTIVE_OUT, MAIN_OUT, EOC_OUT, OUTPUT_COUNT };

  enum ParameterIds {
    DURATION_KNOB,
    DURATION_RANGE_SWITCH,
    SUSTAIN_MODE_SWITCH,
    PARAMETER_COUNT
  };

private:
  void choose_stage_type() {
    auto old_stage_type = stage_type;
    stage_type = stage_type_in() ? SUSTAIN : HOLD;

    // If no change, there's nothing more to do.
    if (stage_type == old_stage_type) {
      return;
    }

    // If no stage in progress, there's nothing more to do.
    if (mode != &generating_mode) {
      return;
    }

    // If we're now holding, continue holding.
    if (stage_type == HOLD) {
      return;
    }

    // If the sustain gate is up, continue sustaining.
    if (sustain_gate_in()) {
      return;
    }

    // The sustain gate is down, so there's no sustain to continue.
    stop_generating();
  }

  void enter(Mode *incoming) {
    mode->exit();
    mode = incoming;
    mode->enter();
  }

  auto envelope_in() const -> float { return inputs[MAIN_IN].value; }

  void stop_generating() {
    eoc_generator.start();
    enter(&following_mode);
  }

  auto modulated(ParameterIds knob_param, InputIds cv_input) const -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    return Knob::modulated(rotation, cv);
  }

  void send_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

  auto stage_type_in() const -> bool {
    return params[SUSTAIN_MODE_SWITCH].value > 0.5f;
  }

  DeferGate<Hostage> defer_gate{this};
  StageGate<Hostage> stage_gate{this};

  StageGenerator<Hostage> stage_generator{this};
  EocGenerator<Hostage> eoc_generator{this};

  DeferringMode<Hostage> deferring_mode{this};
  FollowingMode<Hostage> following_mode{this, &stage_gate};
  GeneratingMode<Hostage> generating_mode{this, &stage_generator, &stage_gate};

  enum StageType { HOLD, SUSTAIN };

  Mode *mode{&following_mode};
  float held_voltage{0.f};
  StageType stage_type{HOLD};
};

struct HostageWidget : public ModuleWidget {
  explicit HostageWidget(rack::Module *module)
      : ModuleWidget(module, 5, "hostage") {
    auto widget_right_edge = width();

    auto left_x = width() / 4.f + 0.333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_switch(Hostage::SUSTAIN_MODE_SWITCH,
                   {center_x, top_row_y + row * row_spacing});

    row++;
    install_input(Hostage::DURATION_CV,
                  {left_x, top_row_y + row * row_spacing});
    install_switch(Hostage::DURATION_RANGE_SWITCH,
                   {right_x, top_row_y + row * row_spacing}, 2, 1);

    row++;
    install_knob("large", Hostage::DURATION_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;
    install_input(Hostage::DEFER_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Hostage::ACTIVE_OUT,
                   {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Hostage::SUSTAIN_GATE_IN,
                  {left_x, top_row_y + row * row_spacing});
    install_output(Hostage::EOC_OUT, {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Hostage::MAIN_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Hostage::MAIN_OUT, {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelHostage =
    rack::Model::create<DHE::Hostage, DHE::HostageWidget>(
        "DHE-Modules", "Hostage", "Hostage", rack::ENVELOPE_GENERATOR_TAG);
