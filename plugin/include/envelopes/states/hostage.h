#pragma once

#include <util/mode.h>
#include <envelopes/components/stage-gate.h>

namespace DHE {
namespace hostage {

template <typename M, typename S>
class State : public DHE::Mode {
public:
  explicit State(M *module, S *states) : module{module}, states{states} {}

  virtual void on_stage_gate_rise() {};
  virtual void on_stage_gate_fall() {};

protected:
  M *const module;
  S *const states;
};

template<typename M, typename S>
class Holding : public State<M,S> {
public:
  explicit Holding(M *module, S *states) : State<M,S>{module, states} {}

  void step() override { this->module->do_hold(); }

  void on_stage_gate_rise() override { this->states->start_holding(); }
};

template<typename M, typename S>
class Resting : public State<M,S> {
public:
  explicit Resting(M *module, S *states) : State<M,S>{module, states} {}

  void step() override { this->module->do_rest(); }

  void on_stage_gate_rise() override {
    this->states->start_generating();
  }
};

template <typename M, typename S> class Sustaining : public State<M, S> {
public:
  explicit Sustaining(M *module, S *states) : State<M,S>{module, states} {}

  void step() override { this->module->do_hold(); }

  void on_stage_gate_fall() override { this->states->start_resting(); }
};

template <typename M>
class StateMachine {
public:
  explicit StateMachine(M *module) : module{module} {}

  void start() { state->enter(); }

  void step() {
    stage_gate.step();
    state->step();
  }

  void start_holding() { enter(&holding); }

  void start_generating() {
    if(module->is_sustain_mode()) {
      enter(&sustaining);
    } else {
      enter(&holding);
    }
  }

  void start_resting() { enter(&resting); }

  void on_stage_gate_rise() { state->on_stage_gate_rise(); }
  void on_stage_gate_fall() { state->on_stage_gate_fall(); }

private:

  void enter(State<M,StateMachine<M>> *incoming) {
    state->exit();
    state = incoming;
    state->enter();
  }

  M *const module;
  State<M,StateMachine<M>> *state{&resting};

  Holding<M,StateMachine<M>> holding{module, this};
  Resting<M,StateMachine<M>> resting{module, this};
  Sustaining<M,StateMachine<M>> sustaining{module, this};
  StageGate<M, StateMachine<M>> stage_gate{module, this};
};
}
}

