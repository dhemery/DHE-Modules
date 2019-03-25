#pragma once

#include "stage-mode.h"

namespace DHE {

template <typename M> class SustainingMode : public StageMode {
public:
  explicit SustainingMode(M *module) : module{module} {}

  void step() override { module->do_sustain(); }

  void on_defer_gate_rise() override { module->start_deferring(); };
  void on_stage_gate_fall() override { module->finish_generating(); };

private:
  M* const module;
};
} // namespace DHE
