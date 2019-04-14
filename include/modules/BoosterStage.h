#pragma once

#include "modules/Module.h"

#include "display/controls.h"
#include "display/panel.h"
#include "envelopes/StageStateMachine.h"
#include "util/duration.h"
#include "util/selector.h"
#include "util/signal.h"

namespace DHE {

class BoosterStage : public Module {
public:
  BoosterStage() : BoosterStage{[]() -> float { return rack::engineGetSampleTime(); }} {}
  explicit BoosterStage(const std::function<float()>& sample_time);

  auto defer_gate_in() const -> bool;
  auto defer_gate_is_active() const -> bool;
  auto duration() const -> float;
  void forward();
  void generate(float phase);
  void prepare_to_generate();
  void set_active_button(bool active);
  void set_active(bool active);
  void set_eoc(bool eoc);
  void set_eoc_button(bool eoc);
  auto stage_gate_in() const -> bool;
  void step() override;

  const Selector<Range const *, 3> duration_range_selector{
      Duration::ranges(), [this](Range const *range) { duration_range = range; }};
  const Selector<Range const *, 2> level_range_selector{
      Signal::ranges(), [this](Range const *range) { level_range = range; }};

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
  auto is_s_shape() const -> bool;
  auto level() const -> float;
  void send_active();
  void send_eoc();
  void send_out(float voltage);
  auto taper(float phase) const -> float;

  StageStateMachine state_machine;
  Range const *duration_range{&Duration::medium_range};
  Range const *level_range{&Signal::bipolar_range};
  bool is_active{false};
  bool active_button_is_pressed{false};
  bool is_eoc{false};
  bool eoc_button_is_pressed{false};
  float start_voltage{0.f};
};
} // namespace DHE
