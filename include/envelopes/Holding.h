#pragma once

#include <functional>
#include <utility>

#include "StageState.h"
#include "components/phase-accumulator.h"

namespace DHE {

class Holding : public StageState {
public:
  Holding(std::function<void()> finish_holding, std::function<float()> duration,
          std::function<float()> sample_time, std::function<void()> forward,
          std::function<void(bool)> set_active)
      : StageState{
            [this]() { start(); },         // Restart on stage gate rise
            []() {},                       // Ignore stage gate fall
            [this]() { start(); },         // Start holding on entry
            [this]() { generator.step(); } // Generate on each step
        },
        set_active{std::move(set_active)}, forward{std::move(forward)},
        generator{std::move(duration), std::move(sample_time), []() {},
                  [](float) {}, std::move(finish_holding)} {}

private:
  void start() {
    set_active(true);
    forward();
    generator.start();
  }

  const std::function<void(bool)> set_active;
  const std::function<void()> forward;
  PhaseAccumulator generator;
};
} // namespace DHE
