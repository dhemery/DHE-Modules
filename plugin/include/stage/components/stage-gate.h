#pragma once
#include "components/gate.h"

namespace DHE {

/**
 * Informs the sink when the source's stage gate signal rises and falls.
 */
template<typename M, typename S>
class StageGate : public Gate {
public:
  explicit StageGate(M *module, S *states)
      : module{module},
        states{states} {}

protected:
  auto state_in() const -> bool override { return module->stage_gate_in(); }

  void on_rise() override { states->on_stage_gate_rise(); }

  void on_fall() override { states->on_stage_gate_fall(); }

private:
  M const *const module;
  S *const states;
};
}
