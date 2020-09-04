#pragma once

#include <array>
#include <ostream>

namespace dhe {
namespace curve_sequencer {
enum class AnchorSource { Level, In, Out };

static auto constexpr anchor_source_count =
    static_cast<int>(AnchorSource::Out) + 1;

static auto constexpr anchor_source_names =
    std::array<char const *, anchor_source_count>{
        "AnchorSource::Level", "AnchorSource::In", "AnchorSource::Out"};

static auto name_of(AnchorSource source) -> char const * {
  return anchor_source_names[static_cast<int>(source)];
}

static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::AnchorSource source)
    -> std::ostream & {
  os << name_of(source);
  return os;
}

template <typename ControlsT> class Anchor {
public:
  Anchor(ControlsT &controls) {}
  void enter(int /*step*/) {}
};
} // namespace curve_sequencer
} // namespace dhe
