#pragma once

#include "modules/curve-sequencer/anchor-source.h"
#include <array>
#include <ostream>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::AnchorSource;

static auto constexpr anchor_sources = std::array<AnchorSource, 3>{
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

static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::AnchorSource source)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(source);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
