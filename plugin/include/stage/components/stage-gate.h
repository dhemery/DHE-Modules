#pragma once
#include "components/gate.h"

namespace DHE {

/**
 * Informs the sink when the source's stage gate signal rises and falls.
 */
template <typename TSource, typename TSink>
class StageGate : public Gate {
public:
  explicit StageGate(TSource *source, TSink *sink) : source{source}, sink{sink} {}

protected:
  auto state_in() const -> bool override { return source->stage_gate_in(); }

  void on_rise() override { sink->on_stage_gate_rise(); }

  void on_fall() override { sink->on_stage_gate_fall(); }

private:
  TSource const * const source;
  TSink * const sink;
};
}
