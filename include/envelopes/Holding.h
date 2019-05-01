#pragma once
#include <functional>
#include <utility>

#include "StageState.h"
#include "components/phase-accumulator.h"

namespace DHE {

class Holding : public StageState {
public:
  Holding(std::function<void()> finish_holding, std::function<float()> duration,
          std::function<void(float)> forward,
          std::function<void(bool)> set_active)
      : StageState{
            [this]() { start(); },         // Restart on stage gate rise
            []() {},                       // Ignore stage gate fall
            [this]() { start(); },         // Start holding on entry
            [this](float sampleTime) { generator.step(sampleTime); } // Generate on each step
        },
        set_active{std::move(set_active)}, forward{std::move(forward)},
        generator{std::move(duration), []() {},
                  [](float) {}, std::move(finish_holding)} {}

private:
  void start() {
    set_active(true);
    forward(0.f);
    generator.start();
  }

  const std::function<void(bool)> set_active;
  const std::function<void(float)> forward;
  PhaseAccumulator generator;
};
} // namespace DHE
