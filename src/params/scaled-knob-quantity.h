#pragma once

#include "knob-quantity.h"

#include "components/range.h"

namespace dhe {

struct ScaledKnobQuantity : public KnobQuantity<float> {
  void set_range(Range r) {
    auto const old_range = Range{minValue, maxValue};
    auto const rotation = old_range.normalize(getValue());
    auto const default_rotation = old_range.normalize(defaultValue);
    minValue = r.lower_bound();
    maxValue = r.upper_bound();
    defaultValue = r.scale(default_rotation);
    setValue(r.scale(rotation));
  }
};

} // namespace dhe
