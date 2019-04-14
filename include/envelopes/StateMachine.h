#pragma once

#include <utility>

#include "components/edge-detector.h"
#include "components/mode.h"

#include "Deferring.h"
#include "EndOfCyclePulseGenerator.h"
#include "Forwarding.h"
#include "Idling.h"
#include "StageState.h"

namespace DHE {

class StateMachine {
public:
  explicit StateMachine(const std::function<float()> &sample_time,
                        std::function<bool()> defer_gate_is_active,
                        std::function<bool()> defer_gate_is_up,
                        const std::function<bool()>& stage_gate_is_up,
                        const std::function<void(bool)> &set_active,
                        const std::function<void(bool)> &set_eoc,
                        const std::function<void()> &forward)
      : defer_gate_is_active{std::move(defer_gate_is_active)},
        stage_gate_is_up{stage_gate_is_up},
        eoc_generator{sample_time, [set_eoc]() { set_eoc(true); },
                      [set_eoc]() { set_eoc(false); }},
        stage_gate{stage_gate_is_up, [this]() { on_stage_gate_rise(); },
                   [this]() { on_stage_gate_fall(); }},
        defer_gate{std::move(defer_gate_is_up), [this]() { enter(&deferring); },
                   [this]() { stop_deferring(); }},
        deferring{set_active, forward}, forwarding{[this]() {
                                                     start_generating();
                                                   },
                                                   set_active, forward},
        idling{[this]() { start_generating(); }, set_active} {}

  void start() { state->enter(); }

  void step() {
    defer_gate.step();
    stage_gate.step();
    state->step();
    eoc_generator.step();
  }

protected:
  virtual void start_generating() = 0;

  void enter(StageState *incoming) {
    state->exit();
    state = incoming;
    state->enter();
  }

  void finish_stage() {
    eoc_generator.start();
    enter(&idling);
  };

private:
  void on_stage_gate_rise() {
    // If DEFER is active, ignore GATE rises.
    // We will check GATE when DEFER falls.
    if (defer_gate_is_active())
      return;
    state->on_stage_gate_rise();
  }

  void on_stage_gate_fall() { state->on_stage_gate_fall(); }

  void stop_deferring() {
    if (stage_gate_is_up()) {
      start_generating();
    } else {
      finish_stage();
    }
  }

  const std::function<bool()> defer_gate_is_active;
  const std::function<bool()> stage_gate_is_up;

  EndOfCyclePulseGenerator eoc_generator;

  StageState *state{&forwarding};

  EdgeDetector stage_gate;

  EdgeDetector defer_gate;

  Deferring deferring;
  Forwarding forwarding;
  Idling idling;
}; // namespace DHE
} // namespace DHE
