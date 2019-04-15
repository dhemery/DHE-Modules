#pragma once

#include <functional>
#include <utility>

#include "StageState.h"

namespace DHE {

/**
 * An idling stage module is inactive and takes no action on each step.
 */
class Idling : public StageState {
public:
  explicit Idling(std::function<void()> on_stage_gate_rise,
                  std::function<void(bool)> const &set_active)
      : StageState{
            std::move(on_stage_gate_rise), // As instructed on stage gate rise
            []() {},                       // Ignore stage gate fall
            [set_active]() { set_active(false); }, // Deactivate on entry
            []() {}                                // Do nothing on each step
        } {}
};
} // namespace DHE
