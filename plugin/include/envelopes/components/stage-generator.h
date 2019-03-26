#pragma once
#include "components/phase-accumulator.h"

namespace DHE {

/**
 * Advances its phase over the duration specified by the module, and informs the
 * module when the phase advances, and the state machine when the phase ends.
 */
template<typename M>
class StageGenerator : public PhaseAccumulator {
public:
  explicit StageGenerator(M *module)
      : module{module} {}

  auto duration() const -> float override { return module->duration(); }

  auto sample_time() const -> float override { return module->sample_time(); }

  void on_advance(float phase) const override { module->generate(phase); }

  void on_finish() const override { module->finish_generating(); }

private:
  M *const module;
};
}
