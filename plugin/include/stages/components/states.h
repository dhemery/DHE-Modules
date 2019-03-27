#pragma once

#include "components/mode.h"
#include "hold-generator.h"
#include "pulse-generator.h"
#include "stage-gate.h"

namespace DHE {
template <typename M>
class StageState : public DHE::Mode {
public:
  explicit StageState(M *module,
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
  void prepare_to_generate() const { module->prepare_to_generate(); }

private:
  M *const module;
};

template <typename M>
class Deferring : public StageState<M> {
public:
  explicit Deferring(M *module)
      : StageState<M>{module, []() {}} {}

  void enter() override {
    this->become_active();
  }
  void step() override {
    this->forward();
  }
};

template <typename M>
class Forwarding : public StageState<M> {
public:
  explicit Forwarding(M *module, std::function<void()> on_stage_gate_rise)
      : StageState<M>{module, on_stage_gate_rise} {}

  void enter() override {
    this->become_inactive();
  }

  void step() override {
    this->forward();
  }
};

template <typename M>
class Generating : public StageState<M> {
public:
  explicit Generating(M *module,
                      PhaseAccumulator *generator,
                      std::function<void()> on_stage_gate_rise)
      : StageState<M>{module, on_stage_gate_rise},
        generator{generator} {}

  void enter() override {
    this->become_active();
    this->prepare_to_generate();
    generator->start();
  }
  void step() override {
    generator->step();
  }

  PhaseAccumulator *generator;
};

template <typename M>
class Holding : public StageState<M> {
public:
  explicit Holding(M *module, PhaseAccumulator *generator, std::function<void()> on_stage_gate_rise)
      : StageState<M>{module, on_stage_gate_rise},
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
class Idling : public StageState<M> {
public:
  explicit Idling(M *module, std::function<void()> on_stage_gate_rise)
      : StageState<M>{module, on_stage_gate_rise} {}

  void enter() override {
    this->become_inactive();
  }
};

template <typename M> class Sustaining : public StageState<M> {
public:
  explicit Sustaining(M *module, std::function<void()> on_stage_gate_fall)
      : StageState<M>{module, []() {}, on_stage_gate_fall} {}

  void enter() override {
    this->become_active();
    this->forward();
  }
};
}
