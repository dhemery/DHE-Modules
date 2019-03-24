#pragma once
#include "components/phase-accumulator.h"

namespace DHE {

/**
 * Generates an end-of-cycle pulse and informs the sink when the pulse rises
 * and falls.
 */
template <typename TSource, typename TSink>
class EocGenerator : public PhaseAccumulator {
public:
  explicit EocGenerator(TSource *source, TSink *sink) : source{source}, sink{sink} {}

  void on_start() const override { sink->on_end_of_cycle_rise(); }

  auto duration() const -> float override { return 1e-3; }

  auto sampleTime() const -> float override { return source->sample_time(); }

  void on_finish() const override { sink->on_end_of_cycle_fall(); }

private:
  TSource const * const source;
  TSink * const sink;
};
}
