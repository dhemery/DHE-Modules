#pragma once

#include <functional>
#include <utility>

#include "StageState.h"
#include "components/phase-accumulator.h"

namespace DHE {

class Generating : public StageState {
public:
  Generating(std::function<void()> finish_generating,
             std::function<float()> duration,
             std::function<float()> sample_time, std::function<void()> prepare,
             std::function<void(float)> generate,
             std::function<void(bool)> set_active)
      : StageState{
            [this]() { start(); },         // Restart on stage gate rise
            []() {},                       // Ignore stage gate fall
            [this]() { start(); },         // Start generating on entry
            [this]() { generator.step(); } // Generate on each step
        },
        set_active{std::move(set_active)}, prepare{std::move(prepare)},
        generator{std::move(duration), std::move(sample_time), []() {},
                  std::move(generate), std::move(finish_generating)} {}

private:
  void start() {
    set_active(true);
    prepare();
    generator.start();
  }

  const std::function<void(bool)> set_active;
  const std::function<void()> prepare;
  PhaseAccumulator generator;
};
} // namespace DHE
