#pragma once

#include "engine.hpp"

#include "util/range.h"
#include "util/signal.h"

namespace DHE {

class Upstage : public rack::Module {
public:
  Upstage();

  void step() override;

  enum ParameterIds {
    LEVEL_KNOB,
    TRIGGER_BUTTON,
    WAIT_BUTTON,
    LEVEL_RANGE_SWITCH,
    PARAMETER_COUNT
  };

  enum InputIds { TRIGGER_IN, WAIT_IN, LEVEL_CV, INPUT_COUNT };

  enum OutputIds { TRIGGER_OUT, MAIN_OUT, OUTPUT_COUNT };

  const Selector<Range const *> level_range_selector{
      Signal::ranges, [this](Range const *range) { level_range = range; }};
private:
  auto envelope_voltage() const -> float;
  auto level() const -> float;
  auto modulated(ParameterIds knob_param, InputIds cv_input) const -> float;
  void send_envelope(float voltage);
  void send_trigger(bool is_triggered);
  auto trigger_in() const -> bool;
  auto wait_in() const -> bool;

  Range const *level_range = &Signal::bipolar_range;
};

} // namespace DHE
