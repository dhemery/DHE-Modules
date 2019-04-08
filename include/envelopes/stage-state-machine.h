#pragma once

#include "envelopes/state-machine.h"

namespace DHE {
template <typename M> class StageGenerator : public PhaseAccumulator {
public:
  StageGenerator(M *module, std::function<void()> on_stage_complete)
      : module{module}, on_stage_complete{std::move(on_stage_complete)} {}

  auto duration() const -> float override { return module->duration(); }

  auto sample_time() const -> float override { return module->sample_time(); }

  void on_advance(float phase) const override { module->generate(phase); }

  void on_finish() const override { on_stage_complete(); }

private:
  M *const module;
  const std::function<void()> on_stage_complete;
};

template <typename M> class Generating : public StageState<M> {
public:
  Generating(M *module, const std::function<void()> &on_stage_gate_rise,
             const std::function<void()> &on_stage_complete)
      : StageState<M>{module, on_stage_gate_rise}, generator{
                                                       module,
                                                       on_stage_complete} {}

  void enter() override {
    this->become_active();
    this->prepare_to_generate();
    generator.start();
  }
  void step() override { generator.step(); }

  StageGenerator<M> generator;
};

template <typename M> class StageStateMachine : public StateMachine<M> {
public:
  explicit StageStateMachine(M *module) : StateMachine<M>{module} {}

protected:
  void start_generating() override { this->enter(&generating); };

private:
  Generating<M> generating{this->module, [this]() { start_generating(); },
                           [this]() { this->finish_stage(); }};
};
} // namespace DHE
