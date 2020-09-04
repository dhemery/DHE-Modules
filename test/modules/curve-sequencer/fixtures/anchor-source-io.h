#pragma once

#include "modules/curve-sequencer/anchor-source.h"
#include <ostream>

namespace dhe {
namespace curve_sequencer {

static auto name_of(dhe::curve_sequencer::AnchorSource source) -> char const * {
  switch (source) {
  case AnchorSource::In:
    return "AnchorSource::In";
  case AnchorSource::Level:
    return "AnchorSource::Level";
  case AnchorSource::Out:
    return "AnchorSource::Out";
  }
}
static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::AnchorSource source)
    -> std::ostream & {
  os << name_of(source);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
