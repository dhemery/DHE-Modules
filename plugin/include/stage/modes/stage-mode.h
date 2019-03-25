#pragma once
#include "util/mode.h"

namespace DHE {

class StageMode : public DHE::Mode {
public:
  explicit StageMode() = default;

  virtual void on_defer_gate_rise() {};
  virtual void on_defer_gate_fall() {};
  virtual void on_stage_gate_rise() {};
  virtual void on_stage_gate_fall() {};
};
}
