#pragma once
#include "components/gate.h"

namespace DHE {

/**
 * Informs the sink when the source's defer gate signal rises or falls.
 */
template <typename TSource, typename TSink>
class DeferGate : public Gate {
public:
  explicit DeferGate(TSource *source, TSink *sink) : source{source}, sink{sink} {}

protected:
  auto state_in() const -> bool override { return source->defer_gate_in(); }

  void on_rise() override { sink->on_defer_gate_rise(); }

  void on_fall() override { sink->on_defer_gate_fall(); }

private:
  TSource const * const source;
  TSink * const sink;
};
}
