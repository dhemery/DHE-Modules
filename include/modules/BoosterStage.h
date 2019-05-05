#pragma once

#include "Module.h"

#include "controls/Duration.h"
#include "envelopes/StageStateMachine.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

class BoosterStage : public Module {
public:
  BoosterStage();
  void process(const ProcessArgs &args) override;

  auto defer_gate_in() const -> bool;
  auto defer_gate_is_active() const -> bool;
  void forward();
  void generate(float phase);
  void prepare_to_generate();
  void set_active_button(bool active);
  void set_active(bool active);
  void set_eoc(bool eoc);
  void set_eoc_button(bool eoc);
  auto stage_gate_in() const -> bool;

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
  auto envelope_in() const -> float;
  auto curvature() const -> float;
  auto level() const -> float;
  void send_active();
  void send_eoc();
  void send_out(float voltage);
  auto taper(float phase) const -> float;

  StageStateMachine state_machine;
  Sigmoid::Shape const *curve_shape{&Sigmoid::j_shape};
  Range const *level_range{&Signal::bipolar_range};
  bool is_active{false};
  bool active_button_is_pressed{false};
  bool is_eoc{false};
  bool eoc_button_is_pressed{false};
  float start_voltage{0.f};
  void set_level_range();
  void set_shape();
  Duration duration;
};
} // namespace DHE
