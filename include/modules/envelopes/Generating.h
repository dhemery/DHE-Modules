#pragma once

#include "StageState.h"

#include "modules/components/PhaseAccumulator.h"

#include <functional>
#include <utility>

namespace dhe {

class Generating : public StageState {
public:
  Generating(std::function<void()> finishGenerating,
             std::function<float()> duration, std::function<void()> prepare,
             std::function<void(float)> generate,
             std::function<void(bool)> setActive)
      : StageState{
            [this]() { start(); }, // Restart on stage gate rise
            []() {},               // Ignore stage gate fall
            [this]() { start(); }, // Start generating on entry
            [this](float sampleTime) {
              generator.step(sampleTime);
            } // Generate on each step
        },
        setActive{std::move(setActive)}, prepare{std::move(prepare)},
        generator{std::move(duration), []() {}, std::move(generate),
                  std::move(finishGenerating)} {}

private:
  void start() {
    setActive(true);
    prepare();
    generator.start();
  }

  const std::function<void(bool)> setActive;
  const std::function<void()> prepare;
  PhaseAccumulator generator;
};
} // namespace dhe
