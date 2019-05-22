#pragma once
#include <functional>
#include <utility>

#include "modules/components/EdgeDetector.h"

#include "Deferring.h"
#include "EndOfCyclePulseGenerator.h"
#include "Forwarding.h"
#include "Idling.h"
#include "StageState.h"

namespace dhe {

class StateMachine {
public:
  StateMachine(std::function<bool()> defer_gate_is_active,
               std::function<bool()> defer_gate_is_up,
               std::function<bool()> const &stage_gate_is_up,
               std::function<void()> const &start_generating,
               std::function<void(bool)> const &set_active,
               std::function<void(bool)> const &set_eoc,
               std::function<void(float)> const &forward);

  void start();
  void step(float sampleTime);

protected:
  void enter(StageState *incoming);
  void finish_stage();

private:
  void on_stage_gate_rise();
  void on_stage_gate_fall();
  void stop_deferring();

  const std::function<bool()> defer_gate_is_active;
  const std::function<bool()> stage_gate_is_up;
  const std::function<void()> start_generating;

  Deferring deferring;
  Forwarding forwarding;
  Idling idling;
  StageState *state{&forwarding};

  EdgeDetector defer_gate;
  EndOfCyclePulseGenerator eoc_generator;
  EdgeDetector stage_gate;
};
} // namespace dhe
