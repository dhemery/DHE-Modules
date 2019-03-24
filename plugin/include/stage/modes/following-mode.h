#pragma once

#include "stage-mode.h"

namespace DHE {

template <typename M> class FollowingMode : public StageMode<M> {
public:
  explicit FollowingMode(M *module) : module{module} {}

  void step() override { module->do_follow(); }

  void on_defer_gate_rise() override { module->begin_deferring(); }

  void on_stage_gate_rise() override { module->begin_generating(); }
private:
  M * const module;
};
} // namespace DHE
