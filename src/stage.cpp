#include <engine.hpp>

#include "dhe-modules.h"
#include "module-widget.h"

#include "util/duration.h"
#include "util/signal.h"

namespace DHE {

class Trigger {
public:
  void step() {
    auto old_state = state;
    state = trigger_in();
    if (state == old_state)
      return;
    if (state) {
      on_rise();
    }
  }
  void set() { state = true; }
  void reset() { state = false; }

protected:
  virtual auto trigger_in() const -> bool = 0;
  virtual void on_rise() = 0;

private:
  bool state = false;
};

class Gate {
public:
  void step() {
    auto old_state = state;
    state = gate_in();
    if (state == old_state)
      return;
    if (state) {
      on_rise();
    } else {
      on_fall();
    }
  }
  void set() { state = true; }
  void reset() { state = false; }

protected:
  virtual auto gate_in() const -> bool = 0;
  virtual void on_rise() = 0;
  virtual void on_fall() = 0;

private:
  bool state = false;
};

class PhaseGenerator {
public:
  void start() {
    accumulated = 0.f;
    on_start();
  }

  void step() {
    accumulated += rack::engineGetSampleTime();
    if (accumulated >= 1.0f) {
      accumulated = 1.f;
    };
    if (accumulated >= 1.0f) {
      on_complete();
    };
  }

  auto phase() const -> float { return this->accumulated; }

protected:
  virtual void on_start(){};
  virtual void on_complete(){};

private:
  float accumulated = 0.f;
};

class Mode {
public:
  virtual void enter(){};
  virtual void step(){};
  virtual void exit(){};
};

template <typename M> class StageGenerator : public PhaseGenerator {
public:
  explicit StageGenerator(M *module) : module{module} {}

  void on_start() override { module->send_active(true); }

  void on_complete() override { module->on_stage_complete(); }

private:
  M *module;
};

template <typename M> class EocGenerator : public PhaseGenerator {
public:
  explicit EocGenerator(M *module) : module{module} {}

  void on_start() override { module->send_eoc(true); }
  void on_complete() override { module->send_eoc(false); }

private:
  M *module;
};

template <typename M> class DeferringMode : public Mode {
public:
  explicit DeferringMode(M *module) : module{module} {}
  void enter() override { module->send_active(true); }
  void step() override { module->send_input(); }

private:
  M *module;
};

template <typename M> class FollowingMode : public Mode {
public:
  explicit FollowingMode(M *module, Trigger *envelope_trigger)
      : module{module}, envelope_trigger{envelope_trigger} {}
  void enter() override { module->send_active(false); }
  void step() override {
    envelope_trigger->step();
    module->send_envelope();
  }

private:
  M *module;
  Trigger *envelope_trigger;
};

template <typename M> class GeneratingMode : public Mode {
public:
  explicit GeneratingMode(M *module, PhaseGenerator *stage_generator,
                          Trigger *envelope_trigger)
      : module{module}, envelope_trigger{envelope_trigger},
        stage_generator{stage_generator} {}

  void enter() override { start(); }

  void step() override {
    stage_generator->step();
    module->send_envelope();
    envelope_trigger->step();
  }

  void start() {
    module->hold_input();
    stage_generator->start();
  }

private:
  M *module;
  Trigger *envelope_trigger;
  PhaseGenerator *stage_generator;
};

template <typename M> class DeferGate : public Gate {
public:
  explicit DeferGate(M *module) : module{module} {}

protected:
  auto gate_in() const -> bool override { return module->defer_in(); }

  void on_rise() override { module->on_defer_rise(); }

  void on_fall() override { module->on_defer_fall(); }

private:
  M *module;
};

template <typename M> class EnvelopeTrigger : public Trigger {
public:
  explicit EnvelopeTrigger(M *module) : module{module} {}

protected:
  auto trigger_in() const -> bool override { return module->trigger_in(); }

  void on_rise() override { module->on_trigger_rise(); }

private:
  M *module;
};

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

  void enter(Mode *incoming) {
    mode->exit();
    mode = incoming;
    mode->enter();
  }

  void hold_input() { held_voltage = envelope_in(); }

  void on_stage_complete() {
    eoc_generator.start();
    enter(&following_mode);
  }

  void on_defer_fall() { enter(&following_mode); }

  void on_defer_rise() { enter(&deferring_mode); }

  void on_trigger_rise() { enter(&generating_mode); }

  void send_active(bool active) {
    outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
  }

  void send_envelope() {
    auto phase = stage_generator.phase();
    send_out(scale(taper(phase), held_voltage, level()));
  }

  void send_input() { send_out(envelope_in()); }

  void send_eoc(bool eoc) { outputs[EOC_OUT].value = eoc ? 10.f : 0.f; }

  auto trigger_in() const -> bool { return inputs[TRIGGER_IN].value > 0.1; }

  enum ParameterIIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { MAIN_IN, TRIGGER_IN, DEFER_IN, INPUT_COUNT };

  enum OutputIds { MAIN_OUT, EOC_OUT, ACTIVE_OUT, OUTPUT_COUNT };

private:
  auto curvature() const -> float {
    auto rotation = params[CURVE_KNOB].value;
    return Sigmoid::curvature(rotation);
  }

  auto duration() const -> float {
    auto rotation = params[DURATION_KNOB].value;
    return DHE::duration(rotation);
  }

  auto envelope_in() const -> float { return inputs[MAIN_IN].value; }

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
    install_input(Stage::MAIN_IN, {left_x, top_row_y + row * row_spacing});
    install_output(Stage::MAIN_OUT, {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelStage = rack::Model::create<DHE::Stage, DHE::StageWidget>(
    "DHE-Modules", "Stage", "Stage", rack::ENVELOPE_GENERATOR_TAG);
