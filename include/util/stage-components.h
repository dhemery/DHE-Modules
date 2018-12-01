#pragma once

#include "phase-accumulator.h"
#include "states.h"

namespace DHE {
template <typename M> class StageGenerator : public PhaseGenerator {
public:
  explicit StageGenerator(M *module) : module{module} {}

  void on_start() override { module->send_active(true); }

  auto duration() const -> float override { return module->duration(); }

  void on_complete() override { module->on_stage_complete(); }

private:
  M *module;
};

template <typename M> class EocGenerator : public PhaseGenerator {
public:
  explicit EocGenerator(M *module) : module{module} {}

  void on_start() override { module->send_eoc(true); }

  auto duration() const -> float override { return 1e-3; }

  void on_complete() override { module->send_eoc(false); }

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
  explicit FollowingMode(M *module, Trigger *envelope_trigger)
      : module{module}, envelope_trigger{envelope_trigger} {}
  void enter() override { module->send_active(false); }
  void step() override {
    envelope_trigger->step();
    module->send_generated();
  }

private:
  M *module;
  Trigger *envelope_trigger;
};

template <typename M> class GeneratingMode : public Mode {
public:
  explicit GeneratingMode(M *module, PhaseGenerator *stage_generator,
                          Trigger *envelope_trigger)
      : module{module}, envelope_trigger{envelope_trigger},
        stage_generator{stage_generator} {}

  void enter() override { start(); }

  void step() override {
    stage_generator->step();
    module->send_generated();
    envelope_trigger->step();
  }

  void start() {
    module->hold_input();
    stage_generator->start();
  }

private:
  M *module;
  Trigger *envelope_trigger;
  PhaseGenerator *stage_generator;
};

template <typename M> class DeferGate : public Gate {
public:
  explicit DeferGate(M *module) : module{module} {}

protected:
  auto gate_in() const -> bool override { return module->defer_in(); }

  void on_rise() override { module->on_defer_rise(); }

  void on_fall() override { module->on_defer_fall(); }

private:
  M *module;
};

template <typename M> class EnvelopeTrigger : public Trigger {
public:
  explicit EnvelopeTrigger(M *module) : module{module} {}

protected:
  auto trigger_in() const -> bool override { return module->trigger_in(); }

  void on_rise() override { module->on_trigger_rise(); }

private:
  M *module;
};
} // namespace DHE