#pragma once

#include "StageState.h"

#include <functional>
#include <utility>

namespace dhe {

class Deferring : public StageState {
public:
  explicit Deferring(std::function<void(float)> const &forward,
                     std::function<void(bool)> const &set_active)
      : StageState{
            []() {},                              // Ignore stage gate rise
            []() {},                              // Ignore stage gate fall
            [set_active]() { set_active(true); }, // Become active on entry
            forward                               // Forward on each step
        } {}
};
} // namespace dhe
