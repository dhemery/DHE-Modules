#pragma once

#include "stage-mode.h"

namespace DHE {

template <typename M> class GeneratingMode : public StageMode {
public:
  explicit GeneratingMode(M *module) : module{module} {}

  void step() override { module->do_generate(); }

  void on_defer_gate_rise() override { module->start_deferring(); };
  void on_stage_gate_rise() override { module->start_generating(); }
  void on_end_of_cycle_rise() override { module->stop_generating(); }

private:
  M* const module;
};

} // namespace DHE
