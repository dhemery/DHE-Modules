#pragma once

#include "modules/cv-sequencer/anchor.h"
#include <array>
#include <ostream>
#include <string>

namespace test {
namespace cv_sequencer {
using dhe::cv_sequencer::anchor_mode_count;
using dhe::cv_sequencer::anchor_source_count;
using dhe::cv_sequencer::anchor_type_count;
using dhe::cv_sequencer::AnchorMode;
using dhe::cv_sequencer::AnchorSource;
using dhe::cv_sequencer::AnchorType;

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
    AnchorMode::Sample,
    AnchorMode::Track,
};

static inline auto name_of(AnchorMode mode) -> std::string {
  switch (mode) {
  case AnchorMode::Sample:
    return "AnchorMode::Sample";
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
} // namespace cv_sequencer
} // namespace test

namespace dhe {
namespace cv_sequencer {

static inline auto operator<<(std::ostream &os, AnchorMode mode)
    -> std::ostream & {
  os << test::cv_sequencer::name_of(mode);
  return os;
}

static inline auto operator<<(std::ostream &os,
                              dhe::cv_sequencer::AnchorSource source)
    -> std::ostream & {
  os << test::cv_sequencer::name_of(source);
  return os;
}

} // namespace cv_sequencer
} // namespace dhe
