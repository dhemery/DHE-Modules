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
  explicit Deferring(std::function<void()> forward,
                     std::function<void(bool)> set_active)
      : set_active{std::move(set_active)}, forward{std::move(forward)} {}

  void enter() override { set_active(true); }
  void step() override { forward(); }

private:
  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
};
} // namespace DHE
