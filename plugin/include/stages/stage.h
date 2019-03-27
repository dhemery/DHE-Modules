#pragma once

#include <functional>

#include <components/mode.h>
#include <components/phase-accumulator.h>
#include <stages/components/defer-gate.h>
#include <stages/components/pulse-generator.h>
#include <stages/components/stage-generator.h>
#include <stages/components/stage-trigger.h>

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

  void enter() override {
    this->module->set_active(true);
  }
  void step() override {
    this->module->forward();
  }
};

template <typename M>
class Forwarding : public State<M> {
public:
  explicit Forwarding(M *module, std::function<void()> on_stage_gate_rise)
      : State<M>{module, on_stage_gate_rise} {}

  void enter() override {
    this->module->set_active(false);
  }
  void step() override {
    this->module->forward();
  }
};

template <typename M>
class Generating : public State<M> {
public:
  explicit Generating(M *module, PhaseAccumulator *generator, std::function<void()> on_stage_gate_rise)
      : State<M>{module, on_stage_gate_rise},
        generator{generator} {}

  void enter() override {
    this->module->set_active(true);
    this->module->hold_input();
    generator->start();
  }
  void step() override {
    generator->step();
  }

  PhaseAccumulator *generator;
};

template <typename M>
class Idling : public State<M> {
public:
  explicit Idling(M *module, std::function<void()> on_stage_gate_rise)
      : State<M>{module, on_stage_gate_rise} {}

  void enter() override {
    this->module->set_active(false);
  }
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
    eoc_generator.step();
  }

private:
  void enter(State<M> *incoming) {
    state->exit();
    state = incoming;
    state->enter();
  }

  M *const module;
  State<M> *state{&forwarding};

  const std::function<void()> enter_deferring{[this]() { enter(&deferring); }};
  const std::function<void()> enter_generating{[this]() { enter(&generating); }};
  const std::function<void()> enter_idling{[this]() { enter(&idling); }};
  const std::function<void()> finish_stage{[this]() {
    eoc_generator.start();
    enter_idling();
  }};
  const std::function<void()> on_stage_gate_rise{[this]() { state->on_stage_gate_rise(); }};

  PulseGenerator<M> eoc_generator{
      module,
      [this]() { module->set_eoc(true); },
      [this]() { module->set_eoc(false); },
  };
  StageGenerator<M> stage_generator{module, finish_stage};
  DeferGate<M> defer_gate{module, enter_deferring, enter_idling};
  StageTrigger<M> stage_gate{module, on_stage_gate_rise};

  Deferring<M> deferring{module};
  Forwarding<M> forwarding{module, enter_generating};
  Generating<M> generating{module, &stage_generator, enter_generating};
  Idling<M> idling{module, enter_generating};
};
}
}
