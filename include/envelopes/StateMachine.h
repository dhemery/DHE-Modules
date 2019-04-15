#pragma once

#include <functional>
#include <utility>

#include "components/edge-detector.h"

#include "Deferring.h"
#include "EndOfCyclePulseGenerator.h"
#include "Forwarding.h"
#include "Idling.h"
#include "StageState.h"

namespace DHE {

class StateMachine {
public:
  StateMachine(std::function<float()> sample_time,
               std::function<bool()> defer_gate_is_active,
               std::function<bool()> defer_gate_is_up,
               std::function<bool()> const &stage_gate_is_up,
               std::function<void(bool)> const &set_active,
               std::function<void(bool)> const &set_eoc,
               std::function<void()> const &forward);

  void start();
  void step();

protected:
  virtual void start_generating() = 0;
  void enter(StageState *incoming);
  void finish_stage();

private:
  void on_stage_gate_rise();
  void on_stage_gate_fall();
  void stop_deferring();

  const std::function<bool()> defer_gate_is_active;
  const std::function<bool()> stage_gate_is_up;

  EndOfCyclePulseGenerator eoc_generator;
  EdgeDetector stage_gate;
  EdgeDetector defer_gate;
  Deferring deferring;
  Forwarding forwarding;
  Idling idling;
  StageState *state{&forwarding};
};
} // namespace DHE
