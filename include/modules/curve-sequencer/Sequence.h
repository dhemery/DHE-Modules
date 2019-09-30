#pragma once

#include "modules/components/Latch.h"
#include "modules/curve-sequencer/Step.h"

#include <utility>
#include <vector>

namespace dhe {
namespace curve_sequencer {

  /**
   * Controls the sequence in a CurveSequencer module.
   * @tparam PtrT the type of smart pointer that holds the latches and steps
   */
  template <template <typename> class PtrT> class Sequence {
  public:
    Sequence(PtrT<Latch> &&runLatch, PtrT<Latch> &&gateLatch, std::vector<PtrT<Step>> &&steps) :
        runLatch{std::move(runLatch)}, gateLatch{std::move(gateLatch)}, steps{std::move(steps)} {}

    ~Sequence() = default;

    void process(float sampleTime) {
      runLatch->step();
      gateLatch->step();

      if (!runLatch->isHigh()) {
        return;
      }
      if (gateLatch->isHigh()) {
        steps[0]->process(sampleTime);
      }
    }

  private:
    PtrT<Latch> runLatch;
    PtrT<Latch> gateLatch;
    std::vector<PtrT<Step>> steps;
  };
} // namespace curve_sequencer
} // namespace dhe
