#pragma once
#include <functional>
#include <utility>

#include "StageState.h"

namespace DHE {

class Deferring : public StageState {
public:
  explicit Deferring(std::function<void()> const &forward,
                     std::function<void(bool)> const &set_active)
      : StageState{
            []() {},                              // Ignore stage gate rise
            []() {},                              // Ignore stage gate fall
            [set_active]() { set_active(true); }, // Become active on entry
            forward                               // Forward on each step
        } {}
};
} // namespace DHE
