#pragma once
#include "StageState.h"

#include <functional>
#include <utility>

namespace dhe {

class Sustaining : public StageState {
public:
  Sustaining(const std::function<void()> &finishSustaining, std::function<void(float)> forward,
             std::function<void(bool)> setActive) :
      StageState{
          []() {},               // Ignore stage gate rise
          finishSustaining,      // Finish sustaining on stage gate fall
          [this]() { start(); }, // Start sustaining on entry
          [](float) {}           // Do nothing on each step
      },
      setActive{std::move(setActive)},
      forward{std::move(forward)} {}

  void start() {
    setActive(true);
    forward(0.f);
  }

private:
  const std::function<void(bool)> setActive;
  const std::function<void(float)> forward;
};
} // namespace dhe
