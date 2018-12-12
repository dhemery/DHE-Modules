#include "dhe-modules.h"
#include "module-widget.h"
#include <utility>

#include "display/controls.h"
#include "util/duration.h"
#include "util/rotation.h"
#include "util/signal.h"
#include "util/stage-components.h"

namespace DHE {

class BoosterStage : public rack::Module {
public:
  BoosterStage() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    mode->enter();
  }

  void step() override {
    defer_gate.step();
    mode->step();
    eoc_generator.step();
  }

  auto defer_gate_in() const -> bool {
    auto defer_button = params[DEFER_BUTTON].value > 0.5f;
    auto defer_input = inputs[DEFER_IN].value > 0.1f;
    return defer_button || defer_input;
  }

  auto duration() const -> float {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    auto selection = static_cast<int>(params[DURATION_RANGE_SWITCH].value);
    return DHE::duration(rotation, selection);
  }

  void hold_input() { held_voltage = envelope_in(); }

  void on_defer_gate_rise() { enter(&deferring_mode); }

  void on_defer_gate_fall() { enter(&following_mode); }

  void on_stage_generator_finish() {
    eoc_generator.start();
    enter(&following_mode);
  }

  void on_stage_trigger_rise() { enter(&generating_mode); }

  void send_active(bool active) {
    outputs[ACTIVE_OUT].value = active || active_button() ? 10.f : 0.f;
  }

  void send_eoc(bool eoc) {
    outputs[EOC_OUT].value = eoc || eoc_button() ? 10.f : 0.f;
  }

  void send_input() { send_out(envelope_in()); }

  void send_stage() {
    auto phase = stage_generator.phase();
    send_out(scale(taper(phase), held_voltage, level()));
  }

  auto stage_trigger_in() const -> bool {
    auto trigger_button = params[TRIGGER_BUTTON].value > 0.5;
    auto trigger_input = inputs[TRIGGER_IN].value > 0.1;
    return trigger_button || trigger_input;
  }

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
    DEFER_IN,
    DURATION_CV,
    LEVEL_CV,
    ENVELOPE_IN,
    TRIGGER_IN,
    INPUT_COUNT
  };

  enum OutputIds { ACTIVE_OUT, EOC_OUT, MAIN_OUT, OUTPUT_COUNT };

private:
  auto active_button() const -> bool {
    return params[ACTIVE_BUTTON].value > 0.5;
  }

  auto curvature() const -> float {
    return Sigmoid::curvature(modulated(CURVE_KNOB, CURVE_CV));
  }

  void enter(Mode *incoming) {
    mode->exit();
    mode = incoming;
    mode->enter();
  }

  auto envelope_in() const -> float { return inputs[ENVELOPE_IN].value; }

  auto eoc_button() const -> bool { return params[EOC_BUTTON].value > 0.5f; }

  auto is_s_shape() const -> bool { return params[SHAPE_SWITCH].value > 0.5f; }

  auto level() const -> float {
    auto is_uni = params[LEVEL_RANGE_SWITCH].value > 0.5f;
    auto range = Signal::range(is_uni);
    auto level = modulated(LEVEL_KNOB, LEVEL_CV);
    return range.scale(level);
  }

  auto modulated(ParameterIds knob_param, InputIds cv_input) const -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    return Rotation::modulated(rotation, cv);
  }

  void send_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

  auto taper(float phase) const -> float {
    return Sigmoid::taper(phase, curvature(), is_s_shape());
  }

  DeferGate<BoosterStage> defer_gate{this};
  StageTrigger<BoosterStage> stage_trigger{this};

  StageGenerator<BoosterStage> stage_generator{this};
  EocGenerator<BoosterStage> eoc_generator{this};

  DeferringMode<BoosterStage> deferring_mode{this};
  FollowingMode<BoosterStage> following_mode{this, &stage_trigger};
  GeneratingMode<BoosterStage> generating_mode{this, &stage_generator,
                                               &stage_trigger};
  Mode *mode{&following_mode};
  float held_voltage = 0.f;
};

class BoosterStageKnob : public LargeKnob {
public:
  BoosterStageKnob() : LargeKnob("booster-stage") {}
};

struct BoosterStageWidget
    : public ModuleWidget<BoosterStageWidget, BoosterStage> {
  static constexpr auto resource_name = "booster-stage";

  explicit BoosterStageWidget(BoosterStage *module) : ModuleWidget(module, 8) {
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
    install(column_3, y, knob<BoosterStageKnob>(BoosterStage::LEVEL_KNOB));
    install(column_5, y,
            thumb_switch<ThumbSwitch2>(BoosterStage::LEVEL_RANGE_SWITCH, 1));

    y += dy;
    install(column_1, y, input(BoosterStage::CURVE_CV));
    install(column_3, y, knob<BoosterStageKnob>(BoosterStage::CURVE_KNOB));
    install(column_5, y,
            thumb_switch<ThumbSwitch2>(BoosterStage::SHAPE_SWITCH));

    y += dy;
    install(column_1, y, input(BoosterStage::DURATION_CV));
    install(column_3, y, knob<BoosterStageKnob>(BoosterStage::DURATION_KNOB));
    install(column_5, y,
            thumb_switch<ThumbSwitch3>(BoosterStage::DURATION_RANGE_SWITCH, 1));

    y = 82.f;
    dy = 15.f;

    install(column_1, y, input(BoosterStage::DEFER_IN));
    install(column_2, y, button<Button>(BoosterStage::DEFER_BUTTON));
    install(column_4, y, button<ReverseButton>(BoosterStage::ACTIVE_BUTTON));
    install(column_5, y, output(BoosterStage::ACTIVE_OUT));

    y += dy;
    install(column_1, y, input(BoosterStage::TRIGGER_IN));
    install(column_2, y, button<Button>(BoosterStage::TRIGGER_BUTTON));
    install(column_4, y, button<ReverseButton>(BoosterStage::EOC_BUTTON));
    install(column_5, y, output(BoosterStage::EOC_OUT));

    y += dy;
    install(column_1, y, input(BoosterStage::ENVELOPE_IN));
    install(column_5, y, output(BoosterStage::MAIN_OUT));
  }
};
} // namespace DHE
rack::Model *modelBoosterStage =
    rack::Model::create<DHE::BoosterStage, DHE::BoosterStageWidget>(
        "DHE-Modules", "BoosterStage", "Booster Stage",
        rack::ENVELOPE_GENERATOR_TAG);
