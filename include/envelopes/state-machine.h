#include <utility>

#include <utility>

#pragma once

#include <utility>

#include "components/edge-detector.h"
#include "components/mode.h"
#include "end-of-cycle-pulse-generator.h"

namespace DHE {
class StageState : public DHE::Mode {
public:
  explicit StageState(std::function<void()> on_stage_gate_rise = []() {},
                      std::function<void()> on_stage_gate_fall = []() {})
      : on_stage_gate_rise{std::move(on_stage_gate_rise)},
        on_stage_gate_fall{std::move(on_stage_gate_fall)} {}

  const std::function<void()> on_stage_gate_rise;
  const std::function<void()> on_stage_gate_fall;
};

/**
 * A deferring stage module is active, and steps by forwarding its input
 * signal to its output port.
 */
class Deferring : public StageState {
public:
  explicit Deferring(std::function<void(bool)> set_active,
                     std::function<void()> forward)
      : StageState{}, set_active{std::move(set_active)}, forward{std::move(
                                                             forward)} {}

  void enter() override { set_active(true); }
  void step() override { forward(); }

private:
  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
};

/**
 * A forwarding stage module is active, and steps by forwarding its input
 * signal to its output port.
 */
class Forwarding : public StageState {
public:
  explicit Forwarding(const std::function<void()> &on_stage_gate_rise,
                      std::function<void(bool)> set_active,
                      std::function<void()> forward)
      : StageState{on_stage_gate_rise},
        set_active{std::move(set_active)}, forward{std::move(forward)} {}

  void enter() override { set_active(true); }

  void step() override { forward(); }

private:
  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
};

/**
 * An idling stage module is inactive and takes no action on each step.
 */
class Idling : public StageState {
public:
  explicit Idling(const std::function<void()> &on_stage_gate_rise,
                  std::function<void(bool)> set_active)
      : StageState{on_stage_gate_rise}, set_active{std::move(set_active)} {}

  void enter() override { set_active(false); }

private:
  std::function<void(bool)> set_active;
};

class StateMachine {
public:
  explicit StateMachine(const std::function<float()> &sample_time,
                        std::function<bool()> defer_gate_is_active,
                        std::function<bool()> defer_gate_is_up,
                        const std::function<bool()>& stage_gate_is_up,
                        const std::function<void(bool)> &set_active,
                        const std::function<void(bool)> &set_eoc,
                        const std::function<void()> &forward)
      : defer_gate_is_active{std::move(defer_gate_is_active)},
        stage_gate_is_up{stage_gate_is_up},
        eoc_generator{sample_time, [set_eoc]() { set_eoc(true); },
                      [set_eoc]() { set_eoc(false); }},
        stage_gate{stage_gate_is_up, [this]() { on_stage_gate_rise(); },
                   [this]() { on_stage_gate_fall(); }},
        defer_gate{std::move(defer_gate_is_up), [this]() { enter(&deferring); },
                   [this]() { stop_deferring(); }},
        deferring{set_active, forward}, forwarding{[this]() {
                                                     start_generating();
                                                   },
                                                   set_active, forward},
        idling{[this]() { start_generating(); }, set_active} {}

  void start() { state->enter(); }

  void step() {
    defer_gate.step();
    stage_gate.step();
    state->step();
    eoc_generator.step();
  }

protected:
  virtual void start_generating() = 0;

  void enter(StageState *incoming) {
    state->exit();
    state = incoming;
    state->enter();
  }

  void finish_stage() {
    eoc_generator.start();
    enter(&idling);
  };

private:
  void on_stage_gate_rise() {
    // If DEFER is active, ignore GATE rises.
    // We will check GATE when DEFER falls.
    if (defer_gate_is_active())
      return;
    state->on_stage_gate_rise();
  }

  void on_stage_gate_fall() { state->on_stage_gate_fall(); }

  void stop_deferring() {
    if (stage_gate_is_up()) {
      start_generating();
    } else {
      finish_stage();
    }
  }

  const std::function<bool()> defer_gate_is_active;
  const std::function<bool()> stage_gate_is_up;

  EndOfCyclePulseGenerator eoc_generator;

  StageState *state{&forwarding};

  EdgeDetector stage_gate;

  EdgeDetector defer_gate;

  Deferring deferring;
  Forwarding forwarding;
  Idling idling;
}; // namespace DHE
} // namespace DHE
