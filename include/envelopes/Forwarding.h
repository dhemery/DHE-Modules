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
             std::function<void()> forward,
             std::function<void(bool)> set_active)
      : StageState{std::move(on_stage_gate_rise)},
        set_active{std::move(set_active)}, forward{std::move(forward)} {}

  void enter() override { set_active(true); }

  void step() override { forward(); }

private:
  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
};
} // namespace DHE
