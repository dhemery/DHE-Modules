#pragma once
#include "components/phase-accumulator.h"
#include "stage/stage-state-machine.h"

namespace DHE {

/**
 * Advances its phase over the duration specified by the module, and informs the
 * module when the phase advances, and the state machine when the phase ends.
 */
template<typename M, typename S>
class StageGenerator : public PhaseAccumulator {
public:
  explicit StageGenerator(M *module, S *states)
      : module{module},
        states{states} {}

  auto duration() const -> float override { return module->duration(); }

  auto sample_time() const -> float override { return module->sample_time(); }

  void on_advance(float phase) const override { module->generate(phase); }

  void on_finish() const override { states->on_generator_completed(); }

private:
  M *const module;
  S *const states;
};
}
