#pragma once

#include "stage-mode.h"

namespace DHE {

template <typename M> class GeneratingMode : public StageMode<M> {
public:
  explicit GeneratingMode(M *module)
      : module{module} {}

  void step() override { module->do_generate(); }

  void on_defer_gate_rise() override { module->begin_deferring(); }

  void on_stage_gate_rise() override { module->begin_generating(); }
private:
  M *module;
};

} // namespace DHE
