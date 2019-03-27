#pragma once

#include "components/mode.h"
#include "components/hold-generator.h"
#include "components/pulse-generator.h"
#include "components/stage-gate.h"

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
  void become_active() const { module->set_active(true); }
  void become_inactive() const { module->set_active(false); }
  void forward() const { module->forward(); }

private:
  M *const module;
};

template <typename M>
class Forwarding : public State<M> {
public:
  explicit Forwarding(M *module, std::function<void()> on_stage_gate_rise)
      : State<M>{module, on_stage_gate_rise} {}

  void enter() override {
    this->become_inactive();
  }

  void step() override {
    this->forward();
  }
};

template <typename M>
class Holding : public State<M> {
public:
  explicit Holding(M *module, PhaseAccumulator *generator, std::function<void()> on_stage_gate_rise)
      : State<M>{module, on_stage_gate_rise},
        generator{generator} {}

  void enter() override {
    this->become_active();
    this->forward();
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
    this->become_inactive();
  }
};

template <typename M> class Sustaining : public State<M> {
public:
  explicit Sustaining(M *module, std::function<void()> on_stage_gate_fall)
      : State<M>{module, []() {}, on_stage_gate_fall} {}

  void enter() override {
    this->become_active();
    this->forward();
  }
};

template <typename M>
class StateMachine {
public:
  explicit StateMachine(M *module) : module{module} {}

  void start() { state->enter(); }

  void step() {
    stage_gate.step();
    state->step();
    eoc_generator.step();
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
    eoc_generator.step();
  }

  void on_eoc_rise() {
    module->set_eoc(true);
  }

  void on_eoc_fall() {
    module->set_eoc(false);
  }

  void on_stage_gate_rise() {
    state->on_stage_gate_rise();
  }

  void on_stage_gate_fall() {
    state->on_stage_gate_fall();
  }

  M *const module;
  State<M> *state{&idling};

  void enter_holding() { enter(&holding); };
  void enter_idling() { enter(&idling); };

  void finish_stage() {
    eoc_generator.start();
    enter_idling();
  };

  PulseGenerator<M> eoc_generator{
      module,
      [this]() { on_eoc_rise(); },
      [this]() { on_eoc_fall(); },
  };

  HoldGenerator<M> hold_generator{module, [this]() { finish_stage(); }};

  StageGate<M> stage_gate{module,
                          [this]() { on_stage_gate_rise(); },
                          [this]() { on_stage_gate_fall(); }
  };

  Forwarding<M> forwarding{module, [this]() { start_generating(); }};
  Holding<M> holding{module, &hold_generator, [this]() { enter_holding(); }};
  Idling<M> idling{module, [this]() { start_generating(); }};
  Sustaining<M> sustaining{module, [this]() { finish_stage(); }};
};
}
}
