#include <utility>

#pragma once

#include <functional>
#include <utility>

#include "StageState.h"

namespace DHE {

class Sustaining : public StageState {
public:
  Sustaining(const std::function<void()> &on_stage_gate_fall,
             std::function<void()> forward,
             std::function<void(bool)> set_active)
      : StageState{[]() {}, // Ignore stage gate rise
                   on_stage_gate_fall, // As instructed
                   [this]() { start_sustaining(); }, // Start sustaining on entry
                   [] () {} // Do nothing on each step
                   },
        set_active{std::move(set_active)}, forward{std::move(forward)} {}

  void start_sustaining() {
    set_active(true);
    forward();
  }

private:
  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
};
} // namespace DHE
