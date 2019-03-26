#pragma once

#include <envelopes/components/defer-gate.h>
#include <envelopes/components/stage-trigger.h>
#include <util/mode.h>

namespace DHE {
namespace stage {

template <typename M, typename S>
class State : public DHE::Mode {
public:
  explicit State(M *module, S *states) : module{module}, states{states} {}

  virtual void on_defer_gate_rise() { states->start_deferring(); };
  virtual void on_defer_gate_fall() {};
  virtual void on_stage_gate_rise() {};

protected:
  M *const module;
  S *const states;
};

template<typename M, typename S>
class Deferring : public State<M, S> {
public:
  explicit Deferring(M *module, S *states) : State<M,S>{module, states} {}

  void enter() override { this->module->start_deferring(); }
  void step() override { this->module->do_defer(); }

  void on_defer_gate_fall() override { this->states->start_resting(); }
};

template<typename M, typename S>
class Generating : public State<M, S> {
public:
  explicit Generating(M *module, S *states) : State<M,S>{module, states} {}

  void enter() override { this->module->start_generating(); }
  void step() override { this->module->do_generate(); }

  void on_stage_gate_rise() override { this->states->start_generating(); }
};

template<typename M, typename S>
class Resting : public State<M, S> {
public:
  explicit Resting(M *module, S *states) : State<M,S>{module, states} {}

  void enter() override { this->module->start_resting(); }
  void step() override { this->module->do_rest(); }

  void on_stage_gate_rise() override { this->states->start_generating(); }
};

template <typename M>
class StateMachine {
public:
  explicit StateMachine(M *module) : module{module} {}

  void start() { state->enter(); }

  void start_deferring() { enter(&deferring); }
  void start_generating() { enter(&generating); }
  void start_resting() { enter(&resting); }

  void step()  {
    defer_gate.step();
    stage_gate.step();
    state->step();
  }

  void on_defer_gate_rise() { state->on_defer_gate_rise(); }
  void on_defer_gate_fall() { state->on_defer_gate_fall(); }
  void on_stage_gate_rise() { state->on_stage_gate_rise(); }

private:
  void enter(State<M, StateMachine<M>> *incoming) {
    state->exit();
    state = incoming;
    state->enter();
  }

  M * const module;
  State<M, StateMachine<M>> *state{&resting};

  Deferring<M, StateMachine<M>> deferring{module, this};
  Generating<M, StateMachine<M>> generating{module, this};
  Resting<M, StateMachine<M>> resting{module, this};
  DeferGate<M, StateMachine<M>> defer_gate{module, this};
  StageTrigger<M, StateMachine<M>> stage_gate{module, this};
};
}
}

