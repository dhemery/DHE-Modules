#pragma once
#include "components/gate.h"

namespace DHE {

/**
 * Informs the module when its stage gate signal rises and falls.
 */
template <typename M> class StageGate : public Gate {
public:
  explicit StageGate(M *module) : module{module} {}

protected:
  auto state_in() const -> bool override { return module->sustain_gate_in(); }

  void on_rise() override { module->on_stage_gate_rise(); }

  void on_fall() override { module->on_stage_gate_fall(); }

private:
  M *module;
};
}
