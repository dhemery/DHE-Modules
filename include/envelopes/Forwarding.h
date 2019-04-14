#pragma once

#include <functional>
#include <utility>

#include "Deferring.h"
#include "EndOfCyclePulseGenerator.h"
#include "StageState.h"
#include "components/edge-detector.h"
#include "components/mode.h"

namespace DHE {

/**
 * A forwarding stage module is active, and steps by forwarding its input
 * signal to its output port.
 */
class Forwarding : public StageState {
public:
  Forwarding(const std::function<void()> &on_stage_gate_rise,
             std::function<void(bool)> set_active,
             std::function<void()> forward)
      : StageState{on_stage_gate_rise},
        set_active{std::move(set_active)}, forward{std::move(forward)} {}

  void enter() override { set_active(true); }

  void step() override { forward(); }

private:
  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
};
} // namespace DHE
