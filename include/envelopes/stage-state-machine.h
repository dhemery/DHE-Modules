#include <utility>

#include <utility>

#include <utility>

#include <utility>

#pragma once

#include "envelopes/state-machine.h"

namespace DHE {
class StageGenerator : public PhaseAccumulator {
public:
  StageGenerator(const std::function<float()> &duration,
                 const std::function<float()> &sample_time,
                 const std::function<void(float)> &generate,
                 const std::function<void()> &on_stage_complete)
      : PhaseAccumulator{duration, sample_time, []() {}, generate,
                         on_stage_complete} {}
};

class Generating : public StageState {
public:
  Generating(const std::function<float()> &duration,
             const std::function<float()> &sample_time,
             std::function<void(bool)> set_active,
             std::function<void()> prepare_to_generate,
             const std::function<void(float)> &generate,
             const std::function<void()> &on_stage_gate_rise,
             const std::function<void()> &on_stage_complete)
      : StageState{on_stage_gate_rise}, set_active{std::move(set_active)},
        prepare_to_generate{std::move(prepare_to_generate)},
        generator{duration, sample_time, generate, on_stage_complete} {}

  void enter() override {
    set_active(true);
    prepare_to_generate();
    generator.start();
  }
  void step() override { generator.step(); }

  const std::function<void(bool)> set_active;
  const std::function<void()> prepare_to_generate;
  StageGenerator generator;
};

class StageStateMachine : public StateMachine {
public:
  StageStateMachine(const std::function<bool()> &defer_is_active,
                    const std::function<bool()> &defer_is_up,
                    const std::function<bool()> &stage_is_up,
                    const std::function<float()> &duration,
                    std::function<float()> const &sample_time,
                    const std::function<void(bool)> &set_active,
                    const std::function<void(bool)> &set_eoc,
                    const std::function<void()> &prepare_to_generate,
                    const std::function<void(float)> &generate,
                    const std::function<void()> &forward)
      : StateMachine{sample_time, defer_is_active, defer_is_up, stage_is_up,
                     set_active,  set_eoc,         forward},
        generating{duration,
                   sample_time,
                   set_active,
                   prepare_to_generate,
                   generate,
                   [this]() { start_generating(); },
                   [this]() { this->finish_stage(); }} {}

protected:
  void start_generating() override { this->enter(&generating); };

private:
  Generating generating;
};
} // namespace DHE
