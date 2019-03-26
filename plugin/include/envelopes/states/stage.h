#pragma once

#include <functional>

#include <util/mode.h>

#include <envelopes/components/defer-gate.h>
#include <envelopes/components/stage-trigger.h>

namespace DHE {
namespace stage {

template <typename M>
class State : public DHE::Mode {
public:
  explicit State(M *module, std::function<void()> on_stage_gate_rise)
      : on_stage_gate_rise{std::move(on_stage_gate_rise)},
        module{module} {}

  const std::function<void()> on_stage_gate_rise;

protected:
  M *const module;
};

template <typename M>
class Deferring : public State<M> {
public:
  explicit Deferring(M *module)
      : State<M>{module, []() {}} {}

  void enter() override { this->module->start_deferring(); }
  void step() override { this->module->do_defer(); }
};

template <typename M>
class Generating : public State<M> {
public:
  explicit Generating(M *module, std::function<void()> on_stage_gate_rise)
      : State<M>{module, on_stage_gate_rise} {}

  void enter() override { this->module->start_generating(); }
  void step() override { this->module->do_generate(); }
};

template <typename M>
class Resting : public State<M> {
public:
  explicit Resting(M *module, std::function<void()> on_stage_gate_rise)
      : State<M>{module, on_stage_gate_rise} {}

  void enter() override { this->module->start_resting(); }
  void step() override { this->module->do_rest(); }
};

template <typename M>
class StateMachine {
public:
  explicit StateMachine(M *module) : module{module} {}

  void start() { state->enter(); }

  void step() {
    defer_gate.step();
    stage_gate.step();
    state->step();
  }

private:
  void enter(State<M> *incoming) {
    state->exit();
    state = incoming;
    state->enter();
  }

  M *const module;
  State<M> *state{&resting};

  const std::function<void()> enter_deferring{[this]() { enter(&deferring); }};
  const std::function<void()> enter_generating{[this]() { enter(&generating); }};
  const std::function<void()> enter_resting{[this]() { enter(&resting); }};
  const std::function<void()> on_stage_gate_rise{[this]() { state->on_stage_gate_rise(); }};

  Deferring<M> deferring{module};
  Generating<M> generating{module, enter_generating};
  Resting<M> resting{module, enter_generating};
  DeferGate<M> defer_gate{module, enter_deferring, enter_resting};
  StageTrigger<M> stage_gate{module, on_stage_gate_rise};
};
}
}
