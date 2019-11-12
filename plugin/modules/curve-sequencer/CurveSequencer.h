#pragma once

#include "SequenceMode.h"
#include "components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  template <int N, typename Controls, typename Selector, typename Generating, typename Sustaining>
  class CurveSequencer {
  public:
    CurveSequencer(Controls &controls, Selector &stepSelector, Generating &generating, Sustaining &sustaining) :
        controls{controls}, stepSelector{stepSelector}, generating{generating}, sustaining{sustaining} {}

    void execute(float sampleTime) {
      runLatch.clock(controls.isRunning());
      gateLatch.clock(controls.isGated());

      if (runLatch.isLow()) {
        return;
      }

      do {
        auto const next = executeMode(sampleTime);
        if (next == mode) {
          return;
        }
        updateState(next);
      } while (mode != SequenceMode::Idle);
    }

  private:
    auto executeMode(float sampleTime) -> SequenceMode {
      switch (mode) {
      case SequenceMode::Idle:
        if (gateLatch.isRise()) {
          step = stepSelector.first();
          return step >= 0 ? SequenceMode::Generating : SequenceMode::Idle;
        }
        return SequenceMode::Idle;
      case SequenceMode::Advancing:
        step = stepSelector.successor(step);
        return step >= 0 ? SequenceMode::Generating : SequenceMode::Idle;
      case SequenceMode::Generating:
        return generating.execute(gateLatch, sampleTime);
      case SequenceMode::Sustaining:
        return sustaining.execute(gateLatch);
      default:
        return SequenceMode::Idle;
      }
    }

    void updateState(SequenceMode next) {
      gateLatch.clock(gateLatch.isHigh()); // To remove the edge
      mode = next;
      switch (mode) {
      case SequenceMode::Generating:
        generating.enter(step);
        return;
      case SequenceMode::Sustaining:
        sustaining.enter(step);
        return;
      case SequenceMode::Idle:
      case SequenceMode::Advancing:
        return;
      }
    }

    int step{0};
    Latch runLatch{};
    Latch gateLatch{};
    SequenceMode mode{SequenceMode::Idle};
    Controls &controls;
    Selector &stepSelector;
    Generating &generating;
    Sustaining &sustaining;
  };
} // namespace curve_sequencer
} // namespace dhe
