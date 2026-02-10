#include "signals/voltage.h"
#include "components/range.h"

namespace dhe {

auto Voltage::range(VoltageRangeId id) -> Range {
  return voltage::ranges[static_cast<size_t>(id)];
}

auto Voltage::scale(float normalized, VoltageRangeId range_id) -> float {
  return range(range_id).scale(normalized);
}

auto Voltage::normalize(float scaled, VoltageRangeId range_id) -> float {
  return range(range_id).normalize(scaled);
}
} // namespace dhe
