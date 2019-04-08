#pragma once

#include "components/mode.h"
#include "defer-gate.h"
#include "end-of-cycle-pulse-generator.h"
#include "stage-gate.h"

namespace DHE {
template <typename M> class StageState : public DHE::Mode {
public:
  explicit StageState(M *module,
                      std::function<void()> on_stage_gate_rise = []() {},
                      std::function<void()> on_stage_gate_fall = []() {})
      : on_stage_gate_rise{std::move(on_stage_gate_rise)},
        on_stage_gate_fall{std::move(on_stage_gate_fall)}, module{module} {}

  const std::function<void()> on_stage_gate_rise;
  const std::function<void()> on_stage_gate_fall;

protected:
  void become_active() const { module->set_active(true); }
  void become_inactive() const { module->set_active(false); }
  void forward() const { module->forward(); }
  void prepare_to_generate() const { module->prepare_to_generate(); }

private:
  M *const module;
};

/**
 * A deferring stage module is active, and steps by forwarding its input signal
 * to its output port.
 */
template <typename M> class Deferring : public StageState<M> {
public:
  explicit Deferring(M *module) : StageState<M>{module, []() {}} {}

  void enter() override { this->become_active(); }
  void step() override { this->forward(); }
};

/**
 * A forwarding stage module is active, and steps by forwarding its input signal
 * to its output port.
 */
template <typename M> class Forwarding : public StageState<M> {
public:
  explicit Forwarding(M *module,
                      const std::function<void()> &on_stage_gate_rise)
      : StageState<M>{module, on_stage_gate_rise} {}

  void enter() override { this->become_inactive(); }

  void step() override { this->forward(); }
};

/**
 * An idling stage module is inactive and takes no action on each step.
 */
template <typename M> class Idling : public StageState<M> {
public:
  explicit Idling(M *module, const std::function<void()> &on_stage_gate_rise)
      : StageState<M>{module, on_stage_gate_rise} {}

  void enter() override { this->become_inactive(); }
};

template <typename M> class StateMachine {
public:
  explicit StateMachine(M *module) : module{module} {}

  void start() { state->enter(); }

  void step() {
    defer_gate.step();
    stage_gate.step();
    state->step();
    eoc_generator.step();
  }

protected:
  virtual void start_generating() = 0;

  void enter(StageState<M> *incoming) {
    state->exit();
    state = incoming;
    state->enter();
  }

  void finish_stage() {
    eoc_generator.start();
    enter(&idling);
  };

  M *const module;

private:
  void on_defer_gate_rise() { enter(&deferring); }
  void on_defer_gate_fall() { stop_deferring(); }
  void on_stage_gate_rise() {
    // If DEFER is active, suppress GATE rises.
    // We will check GATE when DEFER falls.
    if (!module->defer_gate_is_active()) {
      state->on_stage_gate_rise();
    }
  }
  void on_stage_gate_fall() { state->on_stage_gate_fall(); }
  void on_eoc_rise() { module->set_eoc(true); }
  void on_eoc_fall() { module->set_eoc(false); }

  void stop_deferring() {
    if (module->stage_gate_in()) {
      start_generating();
    } else {
      finish_stage();
    }
  }

  StageState<M> *state{&forwarding};

  StageGate<M> stage_gate{module, [this]() { on_stage_gate_rise(); },
                          [this]() { on_stage_gate_fall(); }};

  DeferGate<M> defer_gate{module, [this]() { on_defer_gate_rise(); },
                          [this]() { on_defer_gate_fall(); }};

  EndOfCyclePulseGenerator<M> eoc_generator{
      module,
      [this]() { on_eoc_rise(); },
      [this]() { on_eoc_fall(); },
  };

  Deferring<M> deferring{module};
  Forwarding<M> forwarding{module, [this]() { start_generating(); }};
  Idling<M> idling{module, [this]() { start_generating(); }};
};
} // namespace DHE
