#pragma once

#include "modules/components/Latch.h"
#include "modules/curve-sequencer/Step.h"

#include <memory>
#include <utility>
#include <vector>

namespace dhe {
namespace curve_sequencer {

  /**
   * Controls the sequence in a CurveSequencer module.
   */
  class Sequence {
  public:
    Sequence(Latch &runLatch, Latch &gateLatch, std::function<int()> selectionStart,
             std::function<int()> selectionLength, std::vector<std::unique_ptr<Step>> &steps) :
        runLatch{runLatch},
        gateLatch{gateLatch},
        selectionStart{std::move(selectionStart)},
        selectionLength{std::move(selectionLength)},
        steps{steps} {}

    ~Sequence() = default;

    void process(float sampleTime) {
      runLatch.step();
      gateLatch.step();

      if (runLatch.isLow()) {
        return;
      }
      if (gateLatch.isRise()) {
        start(sampleTime);
      }
    }

  private:
    void start(float sampleTime) {
      auto const first = selectionStart();
      auto const length = selectionLength();

      for (int i = 0; i < length; i++) {
        auto &step = steps[first + i];
        if (step->isAvailable()) {
          step->process(sampleTime);
          return;
        }
      }
    }

    Latch &runLatch;
    Latch &gateLatch;
    std::function<int()> selectionStart;
    std::function<int()> selectionLength;
    std::vector<std::unique_ptr<Step>> &steps;
  };
} // namespace curve_sequencer
} // namespace dhe
