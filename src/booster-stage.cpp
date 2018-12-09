#include "dhe-modules.h"
#include "module-widget.h"

#include "util/duration.h"
#include "util/knob.h"
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
    return Knob::modulated(rotation, cv);
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

struct BoosterStageWidget : public ModuleWidget<BoosterStageWidget, BoosterStage> {
  static constexpr auto resource_name = "booster-stage";

  explicit BoosterStageWidget(BoosterStage *module)
      : ModuleWidget(module, 8) {
    auto widget_right_edge = width();

    auto left_x = widget_right_edge / 6.f + 0.3333333f;
    auto center_x = widget_right_edge / 2.f;
    auto right_x = widget_right_edge - left_x;
    auto button_port_distance = 7.891f;
    auto center_left_x = left_x + button_port_distance;
    auto center_right_x = right_x - button_port_distance;

    auto top_row_y = 25.f;
    auto row_spacing = 18.5f;

    auto row = 0;
    install_input(BoosterStage::LEVEL_CV, {left_x, top_row_y + row * row_spacing});
    install_large_knob(BoosterStage::LEVEL_KNOB,
                 {center_x, top_row_y + row * row_spacing});
    addParam(ThumbSwitch2<BoosterStageWidget>::create(module, BoosterStage::LEVEL_RANGE_SWITCH,
                   {right_x, top_row_y + row * row_spacing}, 1));

    row++;
    install_input(BoosterStage::CURVE_CV, {left_x, top_row_y + row * row_spacing});
    install_large_knob(BoosterStage::CURVE_KNOB,
                 {center_x, top_row_y + row * row_spacing});
    addParam(ThumbSwitch2<BoosterStageWidget>::create(module, BoosterStage::SHAPE_SWITCH,
                   {right_x, top_row_y + row * row_spacing}));

    row++;
    install_input(BoosterStage::DURATION_CV, {left_x, top_row_y + row * row_spacing});
    install_large_knob(BoosterStage::DURATION_KNOB,
                 {center_x, top_row_y + row * row_spacing});
    addParam(ThumbSwitch3<BoosterStageWidget>::create(module, BoosterStage::DURATION_RANGE_SWITCH,
                   {right_x, top_row_y + row * row_spacing}, 1));

    top_row_y = 82.f;
    row_spacing = 15.f;

    row = 0;
    install_input(BoosterStage::DEFER_IN, {left_x, top_row_y + row * row_spacing});
    install_button("normal", BoosterStage::DEFER_BUTTON,
                   {center_left_x, top_row_y + row * row_spacing});
    install_button("reverse", BoosterStage::ACTIVE_BUTTON,
                   {center_right_x, top_row_y + row * row_spacing});
    install_output(BoosterStage::ACTIVE_OUT, {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(BoosterStage::TRIGGER_IN, {left_x, top_row_y + row * row_spacing});
    install_button("normal", BoosterStage::TRIGGER_BUTTON,
                   {center_left_x, top_row_y + row * row_spacing});
    install_button("reverse", BoosterStage::EOC_BUTTON,
                   {center_right_x, top_row_y + row * row_spacing});
    install_output(BoosterStage::EOC_OUT, {right_x, top_row_y + row * row_spacing});

    row++;
    install_input(BoosterStage::ENVELOPE_IN, {left_x, top_row_y + row * row_spacing});
    install_output(BoosterStage::MAIN_OUT, {right_x, top_row_y + row * row_spacing});
  }
};
} // namespace DHE
rack::Model *modelBoosterStage =
    rack::Model::create<DHE::BoosterStage, DHE::BoosterStageWidget>(
        "DHE-Modules", "BoosterStage", "Booster Stage",
        rack::ENVELOPE_GENERATOR_TAG);
