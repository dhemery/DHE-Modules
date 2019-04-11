#pragma once

#include "envelopes/state-machine.h"

namespace DHE {
template <typename M> class StageGenerator : public PhaseAccumulator {
public:
  StageGenerator(M *module, const std::function<float()> &sample_time,
                 std::function<void()> on_stage_complete)
      : PhaseAccumulator{sample_time}, module{module},
        on_stage_complete{std::move(on_stage_complete)} {}

  auto duration() const -> float override { return module->duration(); }

  void on_advance(float phase) const override { module->generate(phase); }

  void on_finish() const override { on_stage_complete(); }

private:
  M *const module;
  const std::function<void()> on_stage_complete;
};

template <typename M> class Generating : public StageState<M> {
public:
  Generating(M *module, const std::function<float()> &sample_time,
             const std::function<void()> &on_stage_gate_rise,
             const std::function<void()> &on_stage_complete)
      : StageState<M>{module, on_stage_gate_rise}, generator{
                                                       module, sample_time,
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
  StageStateMachine(M *module, std::function<float()> const &sample_time)
      : StateMachine<M>{module, sample_time},
        generating{module, sample_time, [this]() { start_generating(); },
                   [this]() { this->finish_stage(); }} {}

protected:
  void start_generating() override { this->enter(&generating); };

private:
  Generating<M> generating;
};
} // namespace DHE
