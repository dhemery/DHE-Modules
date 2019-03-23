#pragma once
#include "components/phase-accumulator.h"

namespace DHE {

/**
 * Generates an end-of-cycle pulse and informs the module when the pulse rises
 * and falls.
 */
template <typename M> class EocGenerator : public PhaseAccumulator {
public:
  explicit EocGenerator(M *module) : module{module} {}

  void on_start() const override { module->set_eoc(true); }

  auto duration() const -> float override { return 1e-3; }

  auto sampleTime() const -> float override { return module->sampleTime(); }

  void on_finish() const override { module->set_eoc(false); }

private:
  M *module;
};
}
