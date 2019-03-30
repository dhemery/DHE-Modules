#pragma once

#include <stages/components/hold-generator.h>
#include <stages/components/state-machine.h>

namespace DHE {
template <typename M> class Holding : public StageState<M> {
public:
  explicit Holding(M *module, PhaseAccumulator *generator,
                   const std::function<void()>& on_stage_gate_rise)
      : StageState<M>{module, on_stage_gate_rise}, generator{generator} {}

  void enter() override {
    this->become_active();
    this->forward();
    generator->start();
  }
  void step() override { generator->step(); }
  PhaseAccumulator *generator;
};

template <typename M> class Sustaining : public StageState<M> {
public:
  explicit Sustaining(M *module, const std::function<void()>& on_stage_gate_fall)
      : StageState<M>{module, []() {}, on_stage_gate_fall} {}

  void enter() override {
    this->become_active();
    this->forward();
  }
};

template <typename M> class HostageStateMachine : public StateMachine<M> {
public:
  explicit HostageStateMachine(M *module) : StateMachine<M>{module} {}

  Holding<M> holding{this->module, &hold_generator, [this]() { this->enter(&holding); }};
  Sustaining<M> sustaining{this->module, [this]() { this->finish_stage(); }};
  HoldGenerator<M> hold_generator{this->module, [this]() { this->finish_stage(); }};

  void start_generating() {
    if (this->module->is_sustain_mode()) {
      this->enter(&sustaining);
    } else {
      this->enter(&holding);
    }
  }
};
} // namespace DHE
