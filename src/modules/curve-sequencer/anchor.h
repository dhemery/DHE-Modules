#pragma once

namespace dhe {
namespace curve_sequencer {
template <typename ControlsT> class Anchor {
public:
  Anchor(ControlsT &controls) {}
  void enter(int /*step*/) {}
};
} // namespace curve_sequencer
} // namespace dhe
