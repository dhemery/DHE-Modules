#include <zconf.h>
#pragma once

#include <functional>
#include <utility>

#include "StageState.h"

namespace DHE {

/**
 * A deferring stage is active and steps by forwarding its input signal to its
 * output port.
 */
class Deferring : public StageState {
public:
  explicit Deferring(std::function<void()> const &forward,
                     std::function<void(bool)> const &set_active)
      : StageState{
            []() {},                              // Ignore stage gate rise
            []() {},                              // Ignore stage gate fall
            [set_active]() { set_active(true); }, // Activate on entry
            forward                               // Forward on each step
        } {}
};
} // namespace DHE
