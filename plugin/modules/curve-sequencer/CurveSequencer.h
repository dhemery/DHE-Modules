#pragma once

#include "Advancing.h"
#include "CurveSequencerControls.h"
#include "GenerateStage.h"
#include "Idle.h"
#include "SequenceMode.h"
#include "SustainStage.h"
#include "components/Latch.h"
#include "controls/CommonInputs.h"

#include <vector>

namespace dhe {
namespace curve_sequencer {
  template <int N, typename Controls, typename Idle, typename Advancing, typename Generating, typename Sustaining>
  class CurveSequencer {
  public:
    CurveSequencer(Controls &controls, Idle &idle, Advancing &advancing, Generating &generating,
                   Sustaining &sustaining) :
        controls{controls}, idle{idle}, advancing{advancing}, generating{generating}, sustaining{sustaining} {}

    void execute(float sampleTime) {
      runLatch.clock(controls.isRunning());
      gateLatch.clock(controls.isGated());

      if (runLatch.isLow()) {
        return;
      }

      do {
        auto const next = executeMode(sampleTime);
        if (next.mode == mode) {
          return;
        }
        updateState(next);
      } while (mode != SequenceMode::Idle);
    }

  private:
    auto executeMode(float sampleTime) -> SequencerState {
      switch (mode) {
      case SequenceMode::Idle:
        return idle.execute(gateLatch);
      case SequenceMode::Advancing:
        return advancing.execute(step);
      case SequenceMode::Generating:
        return generating.execute(gateLatch, sampleTime);
      case SequenceMode::Sustaining:
        return sustaining.execute(gateLatch);
      default:
        return {SequenceMode::Idle, 0};
      }
    }

    void updateState(SequencerState next) {
      gateLatch.clock(gateLatch.isHigh()); // To remove the edge
      step = next.step;
      mode = next.mode;
      switch (next.mode) {
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
    Idle &idle;
    Advancing &advancing;
    Generating &generating;
    Sustaining &sustaining;
  };
} // namespace curve_sequencer
} // namespace dhe
