#pragma once

#include <components/phase-accumulator.h>
#include "stage-mode.h"

namespace DHE {

template <typename M> class GeneratingMode : public StageMode {
public:
  explicit GeneratingMode(M *module, PhaseAccumulator *generator) : module{module}, generator{generator} {}

  void step() override { generator->step(); }

  void on_defer_gate_rise() override { module->start_deferring(); };
  void on_stage_gate_rise() override { generator->start(); }
  void on_generator_completed() override { module->finish_generating(); }

private:
  M* const module;
  PhaseAccumulator *const generator;
};

} // namespace DHE
