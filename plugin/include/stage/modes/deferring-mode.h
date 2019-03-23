#pragma once

#include "util/mode.h"

namespace DHE {

/**
 * A stage module in deferring mode defers to an upstream module by forwarding
 * its input signal to its output port.
 */
template<typename M> class DeferringMode : public Mode {
public:
  explicit DeferringMode(M *module) : module{module} {}

  void enter() override { module->set_active(true); }

  void step() override { module->send_input(); }

private:
  M *module;
};

} // namespace DHE
