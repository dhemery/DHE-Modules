#pragma once

#include "components/range.h"

#include "rack.hpp"

#include <functional>

namespace dhe {

template <typename TRanges>
struct RangeSwitchQuantity : public rack::engine::SwitchQuantity {
  using TAction = std::function<void(Range)>;

  void setValue(float f) override {
    rack::engine::SwitchQuantity::setValue(f);
    auto const selection = static_cast<int>(f);
    action_(TRanges::ranges()[selection]);
  }

  void on_change(TAction const &action) { action_ = action; }

private:
  std::function<void(Range)> action_ = [](Range) {};
};

} // namespace dhe
