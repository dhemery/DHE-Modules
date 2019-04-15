#include <utility>

#pragma once

#include <functional>
#include <utility>

#include "StageState.h"

namespace DHE {

class Sustaining : public StageState {
public:
  Sustaining(const std::function<void()> &finish_sustaining,
             std::function<void()> forward,
             std::function<void(bool)> set_active)
      : StageState{
            []() {},               // Ignore stage gate rise
            finish_sustaining,     // Finish sustaining on stage gate fall
            [this]() { start(); }, // Start sustaining on entry
            []() {}                // Do nothing on each step
        },
        set_active{std::move(set_active)}, forward{std::move(forward)} {}

  void start() {
    set_active(true);
    forward();
  }

private:
  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
};
} // namespace DHE
