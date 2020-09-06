#pragma once

#include "modules/curve-sequencer/anchor.h"
#include <array>
#include <ostream>
#include <string>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::anchor_mode_count;
using dhe::curve_sequencer::anchor_source_count;
using dhe::curve_sequencer::anchor_type_count;
using dhe::curve_sequencer::AnchorMode;
using dhe::curve_sequencer::AnchorSource;
using dhe::curve_sequencer::AnchorType;

static auto constexpr anchor_types = std::array<AnchorType, anchor_type_count>{
    AnchorType::Start,
    AnchorType::End,
};

static inline auto name_of(AnchorType type) -> std::string {
  switch (type) {
  case AnchorType::Start:
    return "AnchorType::Start";
  case AnchorType::End:
    return "AnchorType::End";
  default:
    return "Unknown AnchorType " + std::to_string(static_cast<int>(type));
  }
}

static auto constexpr anchor_modes = std::array<AnchorMode, anchor_mode_count>{
    AnchorMode::Snap,
    AnchorMode::Track,
};

static inline auto name_of(AnchorMode mode) -> std::string {
  switch (mode) {
  case AnchorMode::Snap:
    return "AnchorMode::Snap";
  case AnchorMode::Track:
    return "AnchorMode::Track";
  default:
    return "Unknown AnchorMode " + std::to_string(static_cast<int>(mode));
  }
}

static auto constexpr anchor_sources =
    std::array<AnchorSource, anchor_source_count>{
        AnchorSource::In,
        AnchorSource::Level,
        AnchorSource::Out,
    };

static inline auto name_of(AnchorSource source) -> std::string {
  switch (source) {
  case AnchorSource::In:
    return "AnchorSource::In";
  case AnchorSource::Level:
    return "AnchorSource::Level";
  case AnchorSource::Out:
    return "AnchorSource::Out";
  default:
    return "Unknown AnchorSource " + std::to_string(static_cast<int>(source));
  }
}
} // namespace curve_sequencer
} // namespace test

namespace dhe {
namespace curve_sequencer {

static inline auto operator<<(std::ostream &os, AnchorMode mode)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(mode);
  return os;
}

static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::AnchorSource source)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(source);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
