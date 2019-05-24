#pragma once
#include "StageState.h"
#include "modules/components/PhaseAccumulator.h"

#include <functional>
#include <utility>

namespace dhe {

class Holding : public StageState {
public:
  Holding(std::function<void()> finishHolding, std::function<float()> duration, std::function<void(float)> forward,
          std::function<void(bool)> setActive) :
      StageState{
          [this]() { start(); },                                   // Restart on stage gate rise
          []() {},                                                 // Ignore stage gate fall
          [this]() { start(); },                                   // Start holding on entry
          [this](float sampleTime) { generator.step(sampleTime); } // Generate on each step
      },
      setActive{std::move(setActive)},
      forward{std::move(forward)},
      generator{std::move(duration), []() {}, [](float) {}, std::move(finishHolding)} {}

private:
  void start() {
    setActive(true);
    forward(0.f);
    generator.start();
  }

  const std::function<void(bool)> setActive;
  const std::function<void(float)> forward;
  PhaseAccumulator generator;
};
} // namespace dhe
