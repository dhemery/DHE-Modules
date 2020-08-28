#pragma once

#include "helpers/enums.h"
#include "modules/curve-sequencer-2/triggers.h"
#include <array>

namespace test {
namespace curve_sequencer_2 {
using TriggerMode = dhe::curve_sequencer_2::TriggerMode;
using TriggerModeIterator =
    Iterator<TriggerMode, TriggerMode::GateRises, TriggerMode::GateIsLow>;

static auto constexpr trigger_mode_names =
    std::array<char const *, dhe::curve_sequencer_2::trigger_mode_count>{
        "GateRises", "GateFalls", "GateChanges", "GateIsHigh", "GateIsLow"};

static auto constexpr as_index(TriggerMode mode) -> std::size_t {
  return static_cast<std::size_t>(mode);
}

static auto name_of(TriggerMode mode) -> char const * {
  return trigger_mode_names[as_index(mode)];
}
} // namespace curve_sequencer_2
} // namespace test

static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer_2::TriggerMode mode)
    -> std::ostream & {
  os << test::curve_sequencer_2::name_of(mode);
  return os;
}
