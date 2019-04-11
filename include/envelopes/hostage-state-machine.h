#pragma once

#include <utility>

#include "envelopes/state-machine.h"

namespace DHE {

class HoldGenerator : public PhaseAccumulator {
public:
  HoldGenerator(std::function<float()> duration,
                const std::function<float()> &sample_time,
                const std::function<void()> &on_hold_complete)
      : PhaseAccumulator{std::move(duration), sample_time, []() {},
                         [](float phase) {}, on_hold_complete} {}
};

class Holding : public StageState {
public:
  Holding(std::function<float()> duration,
          const std::function<float()> &sample_time,
          std::function<void(bool)> set_active, std::function<void()> forward,
          const std::function<void()> &on_stage_gate_rise,
          const std::function<void()> &on_stage_complete)
      : StageState{on_stage_gate_rise}, set_active{std::move(set_active)},
        forward{std::move(forward)}, generator{std::move(duration), sample_time,
                                               on_stage_complete} {}

  void enter() override {
    set_active(true);
    forward();
    generator.start();
  }
  void step() override { generator.step(); }

  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
  HoldGenerator generator;
};

class Sustaining : public StageState {
public:
  Sustaining(std::function<void(bool)> set_active,
             std::function<void()> forward,
             const std::function<void()> &on_stage_gate_fall)
      : StageState{[]() {}, on_stage_gate_fall},
        set_active{std::move(set_active)}, forward{std::move(forward)} {}

  void enter() override {
    set_active(true);
    forward();
  }

private:
  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
};

class HostageStateMachine : public StateMachine {
public:
  HostageStateMachine(const std::function<float()> &duration,
                      const std::function<float()> &sample_time,
                      const std::function<void(bool)> &set_active,
                      const std::function<void(bool)> &set_eoc,
                      const std::function<bool()> &defer_is_active,
                      const std::function<bool()> &defer_is_up,
                      const std::function<bool()> &stage_is_up,
                      std::function<bool()> is_sustain_mode,
                      const std::function<void()> &forward)
      : StateMachine{sample_time, defer_is_active, defer_is_up, stage_is_up,
                     set_active,  set_eoc,         forward},
        is_sustain_mode{std::move(is_sustain_mode)}, holding{duration,
                                                             sample_time,
                                                             set_active,
                                                             forward,
                                                             [this]() {
                                                               enter(&holding);
                                                             },
                                                             [this]() {
                                                               finish_stage();
                                                             }},
        sustaining{set_active, forward, [this]() { finish_stage(); }} {}

protected:
  void start_generating() override {
    if (is_sustain_mode()) {
      this->enter(&sustaining);
    } else {
      this->enter(&holding);
    }
  }

private:
  const std::function<bool()> is_sustain_mode;
  Holding holding;
  Sustaining sustaining;
}; // namespace DHE
} // namespace DHE
