#pragma once

#include "stage-mode.h"

namespace DHE {

template <typename M> class RestingMode : public StageMode {
public:
  explicit RestingMode(M *module) : module{module} {}

  void step() override { module->do_rest(); }

  void on_defer_gate_rise() override { module->start_deferring(); };
  void on_stage_gate_rise() override { module->start_generating(); }

private:
  M* const module;
};
} // namespace DHE
