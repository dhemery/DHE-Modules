#pragma once

#include "signals/linear-signals.h"
#include "signals/voltage-signals.h"

#include <array>

namespace dhe {
namespace func {

enum class MultiplierRangeId { Attenuator, Attenuverter, Gain, Gainuverter };

namespace multipliers {
static auto constexpr ranges = std::array<Range, 8>{Attenuator::range,
                                                    Attenuverter::range,
                                                    Gain::range,
                                                    {-2.F, 2.F},
                                                    Range{0.F, 5.F},
                                                    BipolarVoltage::range,
                                                    UnipolarVoltage::range,
                                                    Range{-10.F, 10.F}};

static auto constexpr labels = std::array<char const *, ranges.size()>{
    "0–1", "±1", "0–2", "±2", "0–5", "±5", "0–10", "±10"};

} // namespace multipliers

struct Multiplier {
  static auto constexpr &ranges = multipliers::ranges;

  static inline auto range(MultiplierRangeId id) -> Range {
    return ranges[static_cast<size_t>(id)];
  }

  static inline auto scale(float normalized, MultiplierRangeId range_id)
      -> float {
    return range(range_id).scale(normalized);
  }

  static inline auto normalize(float scaled, MultiplierRangeId range_id)
      -> float {
    return range(range_id).normalize(scaled);
  }
};

} // namespace func
} // namespace dhe
