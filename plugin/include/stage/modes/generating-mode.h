#pragma once

#include "util/mode.h"
#include "components/phase-accumulator.h"
#include "components/trigger.h"

namespace DHE {

template <typename M> class GeneratingMode : public Mode {
public:
  explicit GeneratingMode(M *module)
      : module{module} {}

  void enter() override {
    module->set_active(true);
    module->hold_input();
  }

  void step() override {
    module->do_generate();
  }

private:
  M *module;
};

} // namespace DHE
