#pragma once

#include <functional>
#include <utility>

#include "StageState.h"

namespace DHE {

/**
 * A forwarding stage is active and steps by forwarding its input signal to its
 * output port.
 */
class Forwarding : public StageState {
public:
  Forwarding(std::function<void()> on_stage_gate_rise,
             std::function<void()> const &forward,
             std::function<void(bool)> const &set_active)
      : StageState{
            std::move(on_stage_gate_rise),        // As instructed
            []() {},                              // Ignore stage gate fall
            [set_active]() { set_active(true); }, // Activate on entry
            forward                               // Forward on each step
        } {}
};
} // namespace DHE
