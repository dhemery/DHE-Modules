#pragma once
#include <stage/stage-state-machine.h>
#include "components/phase-accumulator.h"

namespace DHE {

/**
 * Generates an end-of-cycle pulse and informs the module when the pulse rises
 * and falls.
 */
template<typename M>
class EocGenerator : public PhaseAccumulator {
public:
  explicit EocGenerator(M *module)
      : module{module} {}

  void on_start() const override { module->on_end_of_cycle_rise(); }

  auto duration() const -> float override { return 1e-3; }

  auto sample_time() const -> float override { return module->sample_time(); }

  void on_finish() const override { module->on_end_of_cycle_fall(); }

private:
  M *const module;
};
}
