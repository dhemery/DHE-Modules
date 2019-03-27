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
class HostageStateMachine {
public:
  explicit HostageStateMachine(M *module) : module{module} {}

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
    if (module->stage_gate_in()) {
      start_generating();
    } else {
      finish_stage();
    }
  }

  void start_generating() {
    if (module->is_sustain_mode()) {
      enter_sustaining();
    } else {
      enter_holding();
    }
  }

  void enter_holding() { enter(&holding); };
  void enter_idling() { enter(&idling); };
  void enter_sustaining() { enter(&sustaining); };

  void finish_stage() {
    eoc_generator.start();
    enter_idling();
  };

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

  StageGate<M> stage_gate{module,
                          [this]() { on_stage_gate_rise(); },
                          [this]() { on_stage_gate_fall(); }
  };

  DeferGate<M> defer_gate{module,
                          [this]() { on_defer_gate_rise(); },
                          [this]() { on_defer_gate_fall(); }
  };

  HoldGenerator<M> hold_generator{module, [this]() { finish_stage(); }};

  PulseGenerator<M> eoc_generator{
      module,
      [this]() { on_eoc_rise(); },
      [this]() { on_eoc_fall(); },
  };

  Deferring<M> deferring{module};
  Forwarding<M> forwarding{module, [this]() { start_generating(); }};
  Holding<M> holding{module, &hold_generator, [this]() { enter_holding(); }};
  Idling<M> idling{module, [this]() { start_generating(); }};
  Sustaining<M> sustaining{module, [this]() { finish_stage(); }};
};
}
