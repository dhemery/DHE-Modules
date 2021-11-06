#pragma once

#include "components/range.h"

#include "rack.hpp"

#include <functional>

namespace dhe {

struct ScaledQuantity : public rack::engine::ParamQuantity {
  using Action = std::function<void(float)>;

  void setValue(float value) override {
    rack::engine::ParamQuantity::setValue(value);
    action_(value);
  }

  void on_change(Action const &action) { action_ = action; }

  void set_range(Range r) {
    auto const old_range = Range{minValue, maxValue};
    auto const rotation = old_range.normalize(getValue());
    auto const default_rotation = old_range.normalize(defaultValue);
    minValue = r.lower_bound();
    maxValue = r.upper_bound();
    defaultValue = r.scale(default_rotation);
    setValue(r.scale(rotation));
  }

private:
  Action action_ = [](float) {};
};

} // namespace dhe
