#pragma once

#include "Module.h"

#include "modules/controls/Level.h"
#include "util/range.h"
#include "util/signal.h"

namespace DHE {

class Upstage : public Module {
public:
  Upstage();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    LEVEL_KNOB,
    TRIGGER_BUTTON,
    WAIT_BUTTON,
    LEVEL_RANGE_SWITCH,
    PARAMETER_COUNT
  };

  enum InputIds { TRIGGER_IN, WAIT_IN, LEVEL_CV, INPUT_COUNT };

  enum OutputIds { TRIGGER_OUT, MAIN_OUT, OUTPUT_COUNT };

private:
  auto envelope_voltage() const -> float;
  void send_envelope(float voltage);
  void send_trigger(bool is_triggered);
  auto trigger_in() const -> bool;
  auto wait_in() const -> bool;

  Level level;
};

} // namespace DHE
