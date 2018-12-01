#pragma once

#include "phase-accumulator.h"
#include "states.h"

namespace DHE {
template <typename M> class DeferGate : public Gate {
public:
  explicit DeferGate(M *module) : module{module} {}

protected:
  auto state_in() const -> bool override { return module->defer_gate_in(); }

  void on_rise() override { module->start_deferring(); }

  void on_fall() override { module->stop_deferring(); }

private:
  M *module;
};

template <typename M> class SustainGate : public Gate {
public:
  explicit SustainGate(M *module) : module{module} {}

protected:
  auto state_in() const -> bool override { return module->sustain_gate_in(); }

  void on_rise() override { module->start_sustaining(); }

  void on_fall() override { module->stop_sustaining(); }

private:
  M *module;
};

template <typename M> class StageTrigger : public Trigger {
public:
  explicit StageTrigger(M *module) : module{module} {}

protected:
  auto state_in() const -> bool override { return module->stage_trigger_in(); }

  void on_rise() override { module->start_generating(); }

private:
  M *module;
};

template <typename M> class StageGenerator : public PhaseGenerator {
public:
  explicit StageGenerator(M *module) : module{module} {}

  auto duration() const -> float override { return module->duration(); }

  void on_completion() const override { module->finished_generating(); }

private:
  M *module;
};

template <typename M> class EocGenerator : public PhaseGenerator {
public:
  explicit EocGenerator(M *module) : module{module} {}

  void on_start() const override { module->send_eoc(true); }

  auto duration() const -> float override { return 1e-3; }

  void on_completion() const override { module->send_eoc(false); }

private:
  M *module;
};

template <typename M> class DeferringMode : public Mode {
public:
  explicit DeferringMode(M *module) : module{module} {}
  void enter() override { module->send_active(true); }
  void step() override { module->send_input(); }

private:
  M *module;
};

template <typename M> class FollowingMode : public Mode {
public:
  explicit FollowingMode(M *module, Trigger *stage_trigger)
      : module{module}, stage_trigger{stage_trigger} {}
  void enter() override { module->send_active(false); }
  void step() override {
    module->send_stage();
    stage_trigger->step();
  }

private:
  M *module;
  Trigger *stage_trigger;
};

template <typename M> class GeneratingMode : public Mode {
public:
  explicit GeneratingMode(M *module, PhaseGenerator *stage_generator,
                          Trigger *stage_trigger)
      : module{module}, stage_trigger{stage_trigger}, stage_generator{
                                                          stage_generator} {}

  void enter() override {
    module->send_active(true);
    start();
  }

  void step() override {
    stage_generator->step();
    module->send_stage();
    stage_trigger->step();
  }

  void start() {
    module->hold_input();
    stage_generator->start();
  }

private:
  M *module;
  Trigger *stage_trigger;
  PhaseGenerator *stage_generator;
};

template <typename M> class SustainingMode : public Mode {
public:
  explicit SustainingMode(M *module, Gate *sustain_gate)
      : module{module}, sustain_gate{sustain_gate} {}

  void enter() override {
    module->send_active(true);
    module->hold_input();
  }

  void step() override {
    module->send_held();
    sustain_gate->step();
  }

private:
  M *module;
  Gate *sustain_gate;
};
} // namespace DHE
