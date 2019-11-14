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
      resetLatch.clock(controls.isReset());
      if (resetLatch.isRise()) {
        enter(SequenceMode::Idle);
      }

      gateLatch.clock(controls.isGated());

      if (!controls.isRunning()) {
        return;
      }

      do {
        auto const next = executeMode(sampleTime);
        if (next == mode) {
          return;
        }
        enter(next);
      } while (mode != SequenceMode::Idle);
    }

  private:
    auto executeMode(float sampleTime) -> SequenceMode {
      switch (mode) {
      case SequenceMode::Idle:
        if (gateLatch.isRise()) {
          step = stepSelector.first(gateLatch);
          return step >= 0 ? SequenceMode::Generating : SequenceMode::Idle;
        }
        if (resetLatch.isHigh()) {
          controls.output(controls.input());
        }
        return SequenceMode::Idle;
      case SequenceMode::Advancing:
        step = stepSelector.successor(step, gateLatch, controls.isLooping());
        return step >= 0 ? SequenceMode::Generating : SequenceMode::Idle;
      case SequenceMode::Generating:
        return generating.execute(gateLatch, sampleTime);
      case SequenceMode::Sustaining:
        return sustaining.execute(gateLatch);
      default:
        return SequenceMode::Idle;
      }
    }

    void enter(SequenceMode incomingMode) {
      switch (mode) {
      case SequenceMode::Generating:
        generating.exit();
        break;
      case SequenceMode::Sustaining:
        sustaining.exit();
        break;
      default:
        break;
      }

      gateLatch.clock(gateLatch.isHigh()); // To remove the edge

      mode = incomingMode;
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
    Latch gateLatch{};
    Latch resetLatch{};
    SequenceMode mode{SequenceMode::Idle};
    Controls &controls;
    Selector &stepSelector;
    Generating &generating;
    Sustaining &sustaining;
  };
} // namespace curve_sequencer
} // namespace dhe
