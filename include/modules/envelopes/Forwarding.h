#pragma once
#include <functional>
#include <utility>

#include "StageState.h"

namespace dhe {

class Forwarding : public StageState {
public:
  Forwarding(std::function<void()> start_generating,
             std::function<void(float)> const &forward,
             std::function<void(bool)> const &set_active)
      : StageState{
            std::move(start_generating), // Start generating on stage gate rise
            []() {},                     // Ignore stage gate fall
            [set_active]() { set_active(true); }, // Become active on entry
            forward                               // Forward on each step
        } {}
};
} // namespace dhe
