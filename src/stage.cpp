#include <engine.hpp>

#include "dhe-modules.h"
#include "module-widget.h"

#include "util/duration.h"
#include "util/signal.h"
#include "util/stage-components.h"

namespace DHE {

class Stage : public rack::Module {
public:
  Stage()
      : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
        mode{&following_mode} {
    mode->enter();
  }

  void step() override {
    mode_gate.step();
    mode->step();
    eoc_generator.step();
  }

  auto defer_in() const -> bool { return inputs[DEFER_IN].value > 0.1; }

  auto duration() const -> float {
    auto rotation = params[DURATION_KNOB].value;
    return DHE::duration(rotation);
  }

  void enter(Mode *incoming) {
    mode->exit();
    mode = incoming;
    mode->enter();
  }

  void hold_input() { held_voltage = envelope_in(); }

  void on_defer_fall() { enter(&following_mode); }

  void on_defer_rise() { enter(&deferring_mode); }

  void on_stage_complete() {
    eoc_generator.start();
    enter(&following_mode);
  }

  void on_trigger_rise() { enter(&generating_mode); }

  void send_active(bool active) {
    outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
  }

  void send_eoc(bool eoc) { outputs[EOC_OUT].value = eoc ? 10.f : 0.f; }

  void send_generated() {
    auto phase = stage_generator.phase();
    send_out(scale(taper(phase), held_voltage, level()));
  }

  void send_input() { send_out(envelope_in()); }

  auto trigger_in() const -> bool { return inputs[TRIGGER_IN].value > 0.1; }

  enum ParameterIIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { ENVELOPE_IN, TRIGGER_IN, DEFER_IN, INPUT_COUNT };

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

  auto taper(float phase) const -> float {
    return Sigmoid::j_taper(phase, curvature());
  }

  EnvelopeTrigger<Stage> envelope_trigger{this};
  EocGenerator<Stage> eoc_generator{this};
  DeferGate<Stage> mode_gate{this};
  StageGenerator<Stage> stage_generator{this};

  DeferringMode<Stage> deferring_mode{this};
  FollowingMode<Stage> following_mode{this, &envelope_trigger};
  GeneratingMode<Stage> generating_mode{this, &stage_generator,
                                        &envelope_trigger};
  Mode *mode;

  float held_voltage = 0.f;
};

struct StageWidget : public ModuleWidget {
  explicit StageWidget(rack::Module *module)
      : ModuleWidget(module, 5, "stage") {
    auto widget_right_edge = width();

    auto left_x = width() / 4.f + 0.333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_knob("large", Stage::LEVEL_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    row++;
    install_knob("large", Stage::CURVE_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    row++;
    install_knob("large", Stage::DURATION_KNOB,
                 {center_x, top_row_y + row * row_spacing});

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;
    install_input(Stage::DEFER_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Stage::ACTIVE_OUT, {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Stage::TRIGGER_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Stage::EOC_OUT, {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(Stage::ENVELOPE_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Stage::MAIN_OUT, {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelStage = rack::Model::create<DHE::Stage, DHE::StageWidget>(
    "DHE-Modules", "Stage", "Stage", rack::ENVELOPE_GENERATOR_TAG);
