#pragma once

#include "components/range.h"
#include "signals/durations.h"

#include "rack.hpp"

namespace dhe {
class DurationKnobQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    auto const tapered = Duration::taper().apply(rotation);
    return range_.scale(tapered);
  }

  void setDisplayValue(float duration_seconds) override {
    auto const tapered = range_.normalize(duration_seconds);
    auto const rotation = Duration::taper().invert(tapered);
    setValue(rotation);
  }

  void set_display_range(Range r) { range_ = r; }

private:
  Range range_{MediumDuration::range};
};
} // namespace dhe
