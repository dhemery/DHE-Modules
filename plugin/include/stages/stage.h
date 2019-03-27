#pragma once

#include <components/mode.h>
#include <components/phase-accumulator.h>
#include <stages/components/defer-gate.h>
#include <stages/components/pulse-generator.h>
#include <stages/components/stage-generator.h>
#include <stages/components/stage-gate.h>
#include <stages/components/states.h>

namespace DHE {
template <typename M>
class StageStateMachine {
public:
  explicit StageStateMachine(M *module) : module{module} {}

  void start() { state->enter(); }

  void step() {
    defer_gate.step();
    stage_gate.step();
    state->step();
    eoc_generator.step();
  }

private:
  void enter(StageState<M> *incoming) {
    state->exit();
    state = incoming;
    state->enter();
  }

  void enter_deferring() { enter(&deferring); };
  void stop_deferring() {
    if(module->stage_gate_in()) {
      start_generating();
    } else {
      enter_idling();
    }
  }

  void enter_generating() { enter(&generating); };
  void enter_idling() { enter(&idling); };

  void start_generating() { enter_generating(); };
  void finish_stage() {
    eoc_generator.start();
    enter_idling();
  }

  void on_defer_gate_rise() { enter_deferring(); }
  void on_defer_gate_fall() { stop_deferring(); }

  void on_stage_gate_rise() {
    // If DEFER is active, suppress GATE rises.
    // We will check GATE when DEFER falls.
    if(!module->defer_gate_is_active()) {
      state->on_stage_gate_rise();
    }
  }
  void on_stage_gate_fall() { state->on_stage_gate_fall(); }

  void on_eoc_rise() { module->set_eoc(true); }
  void on_eoc_fall() { module->set_eoc(false); }

  M *const module;
  StageState<M> *state{&forwarding};

  DeferGate<M> defer_gate{module,
                          [this]() { on_defer_gate_rise(); },
                          [this]() { on_defer_gate_fall(); }
  };

  StageGate<M> stage_gate{module,
                          [this]() { on_stage_gate_rise(); },
                          [this]() { on_stage_gate_fall(); }
  };

  StageGenerator<M> stage_generator{module, [this]() { finish_stage(); }};

  PulseGenerator<M> eoc_generator{
      module,
      [this]() { on_eoc_rise(); },
      [this]() { on_eoc_fall(); },
  };

  Deferring<M> deferring{module};
  Forwarding<M> forwarding{module, [this]() { start_generating(); }};
  Generating<M> generating{module, &stage_generator, [this]() { start_generating(); }};
  Idling<M> idling{module, [this]() { start_generating(); }};
};
}
