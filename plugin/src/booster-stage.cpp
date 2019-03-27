#include <utility>

#include "engine.hpp"

#include "display/controls.h"
#include "display/panel.h"
#include "stages/stage.h"
#include "util/duration.h"
#include "util/rotation.h"
#include "util/signal.h"

namespace DHE {

class BoosterStage : public rack::Module {
public:
  BoosterStage() : rack::Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    state_machine.start();
  }

  void step() override {
    state_machine.step();
  }

  void forward() {
    send_out(envelope_in());
  }

  void generate(float phase) {
    send_out(scale(taper(phase), start_voltage, level()));
  }

  void hold_input() {
    start_voltage = envelope_in();
  }

  auto duration() const -> float {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    return DHE::duration(rotation, *duration_range);
  }

  auto sample_time() const -> float {
    return rack::engineGetSampleTime();
  };

  auto defer_gate_in() const -> bool {
    auto defer_button = params[DEFER_BUTTON].value > 0.5f;
    auto defer_input = inputs[DEFER_GATE_IN].value > 0.1f;
    return defer_button || defer_input;
  }

  auto stage_gate_in() const -> bool {
    auto trigger_button = params[TRIGGER_BUTTON].value > 0.5;
    auto trigger_input = inputs[STAGE_TRIGGER_IN].value > 0.1;
    return trigger_button || trigger_input;
  }

  void set_active(bool active) {
    is_active = active;
    send_active();
  }

  void set_active_button(bool active) {
    active_button_is_pressed = active;
    send_active();
  }

  void set_eoc(bool eoc) {
    is_eoc = eoc;
    send_eoc();
  }

  void set_eoc_button(bool eoc) {
    eoc_button_is_pressed = eoc;
    send_eoc();
  }

  const Selector<Range const *> duration_range_selector{
      Duration::ranges, [this](Range const *range) { duration_range = range; }};
  const Selector<Range const *> level_range_selector{
      Signal::ranges, [this](Range const *range) { level_range = range; }};

  enum ParameterIds {
    ACTIVE_BUTTON,
    CURVE_KNOB,
    DEFER_BUTTON,
    DURATION_KNOB,
    DURATION_RANGE_SWITCH,
    EOC_BUTTON,
    LEVEL_KNOB,
    LEVEL_RANGE_SWITCH,
    SHAPE_SWITCH,
    TRIGGER_BUTTON,
    PARAMETER_COUNT
  };

  enum InputIds {
    CURVE_CV,
    DEFER_GATE_IN,
    DURATION_CV,
    LEVEL_CV,
    ENVELOPE_IN,
    STAGE_TRIGGER_IN,
    INPUT_COUNT
  };

  enum OutputIds { ACTIVE_OUT, EOC_OUT, MAIN_OUT, OUTPUT_COUNT };

private:
  auto envelope_in() const -> float { return inputs[ENVELOPE_IN].value; }

  auto curvature() const -> float {
    return Sigmoid::curvature(modulated(CURVE_KNOB, CURVE_CV));
  }

  auto is_s_shape() const -> bool { return params[SHAPE_SWITCH].value > 0.5f; }

  auto level() const -> float {
    auto level = modulated(LEVEL_KNOB, LEVEL_CV);
    return level_range->scale(level);
  }

  auto modulated(ParameterIds knob_param, InputIds cv_input) const -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    return Rotation::modulated(rotation, cv);
  }

  void send_active() {
    outputs[ACTIVE_OUT].value =
        is_active || active_button_is_pressed ? 10.f : 0.f;
  }

  void send_eoc() {
    outputs[EOC_OUT].value = is_eoc || eoc_button_is_pressed ? 10.f : 0.f;
  }

  void send_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

  auto taper(float phase) const -> float {
    return Sigmoid::taper(phase, curvature(), is_s_shape());
  }

  stage::StateMachine<BoosterStage> state_machine{this};

  Range const *duration_range{&Duration::medium_range};
  Range const *level_range{&Signal::bipolar_range};
  bool is_active{false};
  bool active_button_is_pressed{false};
  bool is_eoc{false};
  bool eoc_button_is_pressed{false};
  float start_voltage{0.f};
};

class BoosterStagePanel : public Panel<BoosterStagePanel> {
public:
  explicit BoosterStagePanel(BoosterStage *module) : Panel{module, hp} {
    auto widget_right_edge = width();

    auto column_1 = widget_right_edge / 6.f + 0.3333333f;
    auto column_3 = widget_right_edge / 2.f;
    auto column_5 = widget_right_edge - column_1;
    auto button_port_distance = 7.891f;
    auto column_2 = column_1 + button_port_distance;
    auto column_4 = column_5 - button_port_distance;

    auto y = 25.f;
    auto dy = 18.5f;

    install(column_1, y, input(BoosterStage::LEVEL_CV));
    install(column_3, y, knob<LargeKnob>(BoosterStage::LEVEL_KNOB));
    install(column_5, y,
            toggle<2>(BoosterStage::LEVEL_RANGE_SWITCH, 1,
                      module->level_range_selector));

    y += dy;
    install(column_1, y, input(BoosterStage::CURVE_CV));
    install(column_3, y, knob<LargeKnob>(BoosterStage::CURVE_KNOB));
    install(column_5, y, toggle<2>(BoosterStage::SHAPE_SWITCH, 0));

    y += dy;
    install(column_1, y, input(BoosterStage::DURATION_CV));
    install(column_3, y, knob<LargeKnob>(BoosterStage::DURATION_KNOB));
    install(column_5, y,
            toggle<3>(BoosterStage::DURATION_RANGE_SWITCH, 1,
                      module->duration_range_selector));

    y = 82.f;
    dy = 15.f;

    install(column_1, y, input(BoosterStage::DEFER_GATE_IN));
    install(column_2, y, button(BoosterStage::DEFER_BUTTON));
    install(column_4, y,
            button<ReverseButton>(
                BoosterStage::ACTIVE_BUTTON,
                [module](bool active) { module->set_active_button(active); }));
    install(column_5, y, output(BoosterStage::ACTIVE_OUT));

    y += dy;
    install(column_1, y, input(BoosterStage::STAGE_TRIGGER_IN));
    install(column_2, y, button(BoosterStage::TRIGGER_BUTTON));
    install(column_4, y,
            button<ReverseButton>(BoosterStage::EOC_BUTTON, [module](bool eoc) {
              module->set_eoc_button(eoc);
            }));
    install(column_5, y, output(BoosterStage::EOC_OUT));

    y += dy;
    install(column_1, y, input(BoosterStage::ENVELOPE_IN));
    install(column_5, y, output(BoosterStage::MAIN_OUT));
  }

  static constexpr auto module_slug = "booster-stage";

private:
  static constexpr auto hp = 8;
};
} // namespace DHE
rack::Model *modelBoosterStage =
    rack::Model::create<DHE::BoosterStage, DHE::BoosterStagePanel>(
        "DHE-Modules", "BoosterStage", "Booster Stage",
        rack::ENVELOPE_GENERATOR_TAG);
