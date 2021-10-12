#pragma once

#include "./upstage-controls.h"
#include "config/common-config.h"
#include "config/level-config.h"
#include "controls/common-inputs.h"
#include "controls/level-inputs.h"

#include "rack.hpp"

namespace dhe {
namespace stage {

template <typename Controls> class UpstageEngine {
public:
  UpstageEngine(Controls &controls) : controls_{controls} {}

  void process() {
    auto is_triggered = controls_.is_triggered() && !controls_.is_waiting();
    controls_.send_trigger(is_triggered);
    controls_.send_envelope(controls_.level());
  }

private:
  Controls &controls_;
};
} // namespace stage
} // namespace dhe
