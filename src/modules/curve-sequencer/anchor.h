#pragma once

#include <array>
#include <ostream>

namespace dhe {
namespace curve_sequencer {
enum class AnchorSource { Level, In, Out };

static auto constexpr anchor_source_count =
    static_cast<int>(AnchorSource::Out) + 1;

template <typename ControlsT> class Anchor {
public:
  Anchor(ControlsT &controls) {}
  void enter(int /*step*/) {}
};
} // namespace curve_sequencer
} // namespace dhe
