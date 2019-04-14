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
      : StageState{[]() {}, on_stage_gate_fall},
        set_active{std::move(set_active)}, forward{std::move(forward)} {}

  void enter() override {
    set_active(true);
    forward();
  }

private:
  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
};
} // namespace DHE
