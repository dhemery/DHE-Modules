#include "engine.hpp"

#include "display/controls.h"
#include "display/panel.h"
#include "stage/stage-module.h"
#include "util/duration.h"
#include "util/rotation.h"

namespace DHE {

class Hostage : public StageModule {
public:
  Hostage() : StageModule{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  auto duration() const -> float override {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    return DHE::duration(rotation, *duration_range);
  }

  auto level() const -> float override { return held_voltage(); }

  void send_phase(float ignored)  override { send_out(held_voltage()); }

  auto defer_gate_in() const -> bool override { return inputs[DEFER_GATE_IN].value > 0.1f; }

  auto stage_gate_in() const -> bool override { return inputs[STAGE_GATE_IN].value > 0.1f; }

  void set_active(bool active) override {
    outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
  }

  void set_eoc(bool eoc) override { outputs[EOC_OUT].value = eoc ? 10.f : 0.f; }

  auto envelope_in() const -> float override { return inputs[MAIN_IN].value; }

  void send_out(float voltage) override { outputs[MAIN_OUT].value = voltage; }

  void set_duration_range(Range const *range) { duration_range = range; }

  void on_defer_gate_fall() override {
    if(stage_gate_in()) {
      begin_generating();
    } else {
      on_generate_end();
    }
  }

  void on_generate_start() override {
    is_generating = true;
    StageModule::on_generate_start();
  }

  void on_generate_end() override {
    is_generating = false;
    StageModule::on_generate_end();
  }

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
  void choose_stage_type() {
    auto old_stage_type = stage_type;
    stage_type = stage_type_in() ? SUSTAIN : HOLD;

    // If no change, there's nothing more to do.
    if (stage_type == old_stage_type) {
      return;
    }

    // If no stage in progress, there's nothing more to do.
    if (!is_generating) {
      return;
    }

    // If we're holding, continue holding.
    if (stage_type == HOLD) {
      return;
    }

    // If the stage gate is up, continue sustaining.
    if (stage_gate_in()) {
      return;
    }

    // The sustain gate is down, so there's no sustain to continue.
    on_generate_end();
  }

  auto modulated(ParameterIds knob_param, InputIds cv_input) const -> float {
    auto rotation = params[knob_param].value;
    auto cv = inputs[cv_input].value;
    return Rotation::modulated(rotation, cv);
  }

  auto stage_type_in() const -> bool {
    return params[HOSTAGE_MODE_SWITCH].value > 0.5f;
  }

  enum StageType { HOLD, SUSTAIN };

  StageType stage_type{HOLD};
  Range const *duration_range = &Duration::medium_range;
  bool is_generating = false;
};

class HostagePanel : public Panel<HostagePanel> {
public:
  explicit HostagePanel(Hostage *module) : Panel{module, hp} {
    auto widget_right_edge = width();

    auto column_1 = width() / 4.f + 0.333333f;
    auto column_2 = widget_right_edge / 2.f;
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
