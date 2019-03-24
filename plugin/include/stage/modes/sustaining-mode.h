#pragma once

#include "util/mode.h"
#include "components/gate.h"

namespace DHE {

template <typename M> class SustainMode : public Mode {
public:
  explicit SustainMode(M *module)
      : module{module} {}

  void enter() override {
    module->set_active(true);
    module->hold_input();
  }

  void step() override {
    module->do_sustain();
  }

private:
  M *module;
};
} // namespace DHE
