#pragma once

#include "StageState.h"

#include <functional>
#include <utility>

namespace dhe {

class Deferring : public StageState {
public:
  explicit Deferring(std::function<void(float)> const &forward,
                     std::function<void(bool)> const &setActive)
      : StageState{
            []() {},                            // Ignore stage gate rise
            []() {},                            // Ignore stage gate fall
            [setActive]() { setActive(true); }, // Become active on entry
            forward                             // Forward on each step
        } {}
};
} // namespace dhe
