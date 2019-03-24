#pragma once

#include "util/mode.h"

namespace DHE {

template<typename M> class DeferringMode : public Mode {
public:
  explicit DeferringMode(M *module) : module{module} {}

  void enter() override {
    module->set_active(true);
  }

  void step() override {
    module->do_defer();
  }

private:
  M *module;
};

} // namespace DHE
