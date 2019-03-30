#pragma once

#include <components/mode.h>
#include <stages/components/stage-generator.h>
#include <stages/components/state-machine.h>

namespace DHE {
template <typename M> class Generating : public StageState<M> {
public:
  explicit Generating(M *module, PhaseAccumulator *generator,
                      const std::function<void()>& on_stage_gate_rise)
      : StageState<M>{module, on_stage_gate_rise}, generator{generator} {}

  void enter() override {
    this->become_active();
    this->prepare_to_generate();
    generator->start();
  }
  void step() override { generator->step(); }

  PhaseAccumulator *generator;
};

template <typename M> class StageStateMachine : public StateMachine<M> {
public:
  explicit StageStateMachine(M *module) : StateMachine<M>{module} {}

  void start_generating() { this->enter(&generating); };

  StageGenerator<M> stage_generator{this->module, [this]() { this->finish_stage(); }};
  Generating<M> generating{this->module, &stage_generator,
                           [this]() { start_generating(); }};

};
} // namespace DHE
