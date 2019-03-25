#pragma once

#include <stage/components/defer-gate.h>
#include <stage/components/eoc-generator.h>
#include <stage/components/stage-gate.h>
#include <stage/modes/stage-mode.h>

namespace DHE {

template <typename M>
class StageStateMachine {
public:
  explicit StageStateMachine(M *module) : module{module} {}

  void start(StageMode *start_mode) {
    mode = start_mode;
    mode->enter();
  }

  void enter(StageMode *incoming) {
    mode->exit();
    mode = incoming;
    mode->enter();
  }

  void step()  {
    defer_gate.step();
    stage_gate.step();
    mode->step();
    eoc_generator.step();
  }

  void on_defer_gate_rise() { mode->on_defer_gate_rise(); }
  void on_defer_gate_fall() { mode->on_defer_gate_fall(); }
  void on_stage_gate_rise() { mode->on_stage_gate_rise(); }
  void on_stage_gate_fall() { mode->on_stage_gate_fall(); }
  void on_generator_completed() {
    mode->on_generator_completed();
    eoc_generator.start();
  }

private:

  M * const module;
  StageMode *mode;

  EocGenerator<M> eoc_generator{module};
  DeferGate<M, StageStateMachine<M>> defer_gate{module, this};
  StageGate<M, StageStateMachine<M>> stage_gate{module, this};
};
}
