#pragma once

#include "Panel.h"
#include "modules/CurveSequencer8.h"

namespace dhe {

class CurveSequencer8Panel : public Panel<CurveSequencer8Panel> {
public:
  explicit CurveSequencer8Panel(CurveSequencer8 *sequencer);
  static constexpr auto moduleSlug = "curve-sequencer-8";
  static constexpr auto hp = 31;
};
} // namespace dhe
