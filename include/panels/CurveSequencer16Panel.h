#pragma once

#include "Panel.h"
#include "modules/CurveSequencer16.h"

namespace dhe {

class CurveSequencer16Panel : public Panel<CurveSequencer16Panel> {
public:
  explicit CurveSequencer16Panel(CurveSequencer16 *sequencer);
  static constexpr auto moduleSlug = "curve-sequencer-16";
  static constexpr auto hp = 49;
};
} // namespace dhe
