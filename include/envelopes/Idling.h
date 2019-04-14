#pragma once

#include <functional>
#include <utility>

namespace DHE {

/**
 * An idling stage module is inactive and takes no action on each step.
 */
class Idling : public StageState {
public:
  explicit Idling(const std::function<void()> &on_stage_gate_rise,
                  std::function<void(bool)> set_active)
      : StageState{on_stage_gate_rise}, set_active{std::move(set_active)} {}

  void enter() override { set_active(false); }

private:
  std::function<void(bool)> set_active;
};
} // namespace DHE
