#pragma once

#include "stage-mode.h"

namespace DHE {

template<typename M>
class DeferringMode : public StageMode {
public:
  explicit DeferringMode(M *module) : module{module} {}

  void step() override { module->do_defer(); }

  void on_defer_gate_fall() override { module->stop_deferring(); }
private:
  M* const module;
};

} // namespace DHE
