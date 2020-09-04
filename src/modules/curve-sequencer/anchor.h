#pragma once

#include <ostream>

namespace dhe {
namespace curve_sequencer {
enum class AnchorMode { Level, In, Out };

static auto constexpr anchor_mode_count = static_cast<int>(AnchorMode::Out) + 1;

static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::AnchorMode source)
    -> std::ostream & {
  os << static_cast<int>(source);
  return os;
}

template <typename ControlsT> class Anchor {
public:
  Anchor(ControlsT &controls) {}
  void enter(int /*step*/) {}
};
} // namespace curve_sequencer
} // namespace dhe
