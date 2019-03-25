#pragma once

#include <stage/components/stage-generator.h>
#include "stage-mode.h"

namespace DHE {

template <typename M> class GeneratingMode : public StageMode {
public:
  explicit GeneratingMode(M *module) : module{module} {}

  void enter() override { generator.start(); }
  void step() override { generator.step(); }
  void exit() override { generator.stop(); }

  void on_defer_gate_rise() override { module->start_deferring(); };
  void on_stage_gate_rise() override { generator.start(); }

private:
  M* const module;
  StageGenerator<M> generator{module};
};

} // namespace DHE
