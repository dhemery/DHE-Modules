#pragma once
#include "components/phase-accumulator.h"

namespace DHE {

/**
 * Advances its phase over the duration of the stage, and informs the module
 * when the duration ends.
 */
template <typename M> class StageGenerator : public PhaseAccumulator {
public:
  explicit StageGenerator(M *module) : module{module} {}

  void on_start() const override { module->on_generate_start(); }

  auto duration() const -> float override { return module->duration(); }

  auto sampleTime() const -> float override { return module->sampleTime(); }

  void on_step(float phase) const override { module->send_phase(phase); }

  void on_finish() const override { module->on_generate_end(); }

private:
  M *module;
};
}
