#pragma once

namespace dhe {
namespace curve_sequencer {
  /**
   * Controls a single step in a CurveSequencer module.
   * @tparam M the type of module that owns this step
   */
  template <typename M> class Step {
  public:
    Step(M &module, int index) : module{module}, index{index} {}

    void process(float /*sampleTime*/) { module.setGenerating(index, true); }

  private:
    M &module;
    int index;
  };
} // namespace curve_sequencer
} // namespace dhe
