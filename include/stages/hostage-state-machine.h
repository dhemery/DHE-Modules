#pragma once

#include <stages/components/state-machine.h>

namespace DHE {

template <typename M> class HoldGenerator : public PhaseAccumulator {
public:
  HoldGenerator(M *module, std::function<void()> on_hold_complete)
      : module{module}, on_hold_complete{std::move(on_hold_complete)} {}

  auto duration() const -> float override { return module->duration(); }

  auto sample_time() const -> float override { return module->sample_time(); }

  void on_finish() const override { on_hold_complete(); }

private:
  M *const module;
  const std::function<void()> on_hold_complete;
};

template <typename M> class Holding : public StageState<M> {
public:
  Holding(M *module, const std::function<void()> &on_stage_gate_rise,
          const std::function<void()> &on_stage_complete)
      : StageState<M>{module, on_stage_gate_rise}, generator{
                                                       module,
                                                       on_stage_complete} {}

  void enter() override {
    this->become_active();
    this->forward();
    generator.start();
  }
  void step() override { generator.step(); }
  HoldGenerator<M> generator;
};

template <typename M> class Sustaining : public StageState<M> {
public:
  Sustaining(M *module, const std::function<void()> &on_stage_gate_fall)
      : StageState<M>{module, []() {}, on_stage_gate_fall} {}

  void enter() override {
    this->become_active();
    this->forward();
  }
};

template <typename M> class HostageStateMachine : public StateMachine<M> {
public:
  explicit HostageStateMachine(M *module) : StateMachine<M>{module} {}

protected:
  void start_generating() override {
    if (this->module->is_sustain_mode()) {
      this->enter(&sustaining);
    } else {
      this->enter(&holding);
    }
  }

private:
  Holding<M> holding{this->module, [this]() { this->enter(&holding); },
                     [this]() { this->finish_stage(); }};
  Sustaining<M> sustaining{this->module, [this]() { this->finish_stage(); }};
};
} // namespace DHE
