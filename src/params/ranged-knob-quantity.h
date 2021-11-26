#pragma once

#include "components/range.h"

#include "rack.hpp"

#include <functional>

namespace dhe {

template <typename V>
struct RangedKnobQuantity : public rack::engine::ParamQuantity {
  using Action = std::function<void(V)>;

  void set_display_range(Range r) {
    displayMultiplier = r.size();
    displayOffset = r.lower_bound();
  }

  void setValue(float value) override {
    rack::engine::ParamQuantity::setValue(value);
    action_(static_cast<V>(value));
  }

  void on_change(Action const &action) {
    action_ = action;
    action_(static_cast<V>(getValue()));
  }

private:
  Action action_ = [](V /*unused*/) {};
};

} // namespace dhe
