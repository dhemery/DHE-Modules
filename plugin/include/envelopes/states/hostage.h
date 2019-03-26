#pragma once

#include <util/mode.h>
#include <envelopes/components/stage-gate.h>

namespace DHE {
namespace hostage {

template <typename M>
class State : public DHE::Mode {
public:
  explicit State(M *module,
                 std::function<void()> on_stage_gate_rise = []() {},
                 std::function<void()> on_stage_gate_fall = []() {})
      : on_stage_gate_rise{std::move(on_stage_gate_rise)},
        on_stage_gate_fall{std::move(on_stage_gate_fall)},
        module{module} {}

  const std::function<void()> on_stage_gate_rise;
  const std::function<void()> on_stage_gate_fall;

protected:
  M *const module;
};

template <typename M>
class Holding : public State<M> {
public:
  explicit Holding(M *module, std::function<void()> on_stage_gate_rise)
      : State<M>{module, on_stage_gate_rise} {}

  void enter() override { this->module->start_holding(); }
  void step() override { this->module->do_hold(); }
};

template <typename M>
class Resting : public State<M> {
public:
  explicit Resting(M *module, std::function<void()> on_stage_gate_rise)
      : State<M>{module, on_stage_gate_rise} {}

  void enter() override { this->module->start_resting(); }
  void step() override { this->module->do_rest(); }
};

template <typename M> class Sustaining : public State<M> {
public:
  explicit Sustaining(M *module, std::function<void()> on_stage_gate_fall)
      : State<M>{module, []() {}, on_stage_gate_fall} {}

  void enter() override { this->module->start_sustaining(); }
  void step() override { this->module->do_hold(); }
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

  void start_generating() {
    if (module->is_sustain_mode()) {
      enter(&sustaining);
    } else {
      enter(&holding);
    }
  }

private:

  void enter(State<M> *incoming) {
    state->exit();
    state = incoming;
    state->enter();
  }

  M *const module;
  State<M> *state{&resting};

  Holding<M> holding{module, [this]() { enter(&holding); }};
  Resting<M> resting{module, [this]() { start_generating(); }};
  Sustaining<M> sustaining{module, [this]() { enter(&resting); }};
  StageGate<M> stage_gate{module,
                          [this]() { state->on_stage_gate_rise(); },
                          [this]() { state->on_stage_gate_fall(); }
  };
};
}
}

