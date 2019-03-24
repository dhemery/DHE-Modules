#pragma once
#include "components/phase-accumulator.h"

namespace DHE {

/**
 * Advances its phase over the duration specified by the source, and informs the sink
 * when the phase starts, advances, or ends.
 */
template <typename TSource, typename TSink>
class StageGenerator : public PhaseAccumulator {
public:
  explicit StageGenerator(TSource *source, TSink *sink) : source{source}, sink{sink} {}

  void on_start() const override { sink->on_generate_start(); }

  auto duration() const -> float override { return source->duration(); }

  auto sampleTime() const -> float override { return source->sample_time(); }

  void on_step(float phase) const override { sink->send_phase(phase); }

  void on_finish() const override { sink->on_generate_end(); }

private:
  TSource const * const source;
  TSink * const sink;
};
}
