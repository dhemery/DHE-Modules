#pragma once

#include "modules/curve-sequencer/anchor.h"
#include <array>
#include <ostream>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::anchor_mode_count;
using dhe::curve_sequencer::anchor_source_count;
using dhe::curve_sequencer::anchor_type_count;
using dhe::curve_sequencer::AnchorMode;
using dhe::curve_sequencer::AnchorSource;
using dhe::curve_sequencer::AnchorType;

static auto constexpr anchor_types =
    std::array<AnchorType, anchor_type_count>{};
static auto constexpr anchor_modes = std::array<AnchorMode, anchor_mode_count>{
    AnchorMode::Snap,
    AnchorMode::Track,
};

static auto name_of(dhe::curve_sequencer::AnchorMode mode) -> char const * {
  switch (mode) {
  case AnchorMode::Snap:
    return "AnchorMode::Snap";
  case AnchorMode::Track:
    return "AnchorMode::Track";
  }
}

static auto constexpr anchor_sources =
    std::array<AnchorSource, anchor_source_count>{
        AnchorSource::In,
        AnchorSource::Level,
        AnchorSource::Out,
    };

static auto name_of(AnchorSource source) -> char const * {
  switch (source) {
  case AnchorSource::In:
    return "AnchorSource::In";
  case AnchorSource::Level:
    return "AnchorSource::Level";
  case AnchorSource::Out:
    return "AnchorSource::Out";
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
