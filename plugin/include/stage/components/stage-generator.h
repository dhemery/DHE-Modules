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

  auto duration() const -> float override { return module->duration(); }

  auto sampleTime() const -> float override { return module->sampleTime(); }

  void on_finish() const override { module->on_stage_generator_finish(); }

private:
  M *module;
};
}
