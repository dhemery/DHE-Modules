#pragma once

#include "rack.hpp"

#include <functional>

namespace dhe {

template <typename TItems>
struct PickerSwitchQuantity : public rack::engine::SwitchQuantity {
  using Item = typename TItems::ItemType;
  using Action = std::function<void(Item)>;

  void setValue(float f) override {
    rack::engine::SwitchQuantity::setValue(f);
    auto const selection = static_cast<int>(f);
    action_(TItems::items()[selection]);
  }

  void on_change(Action const &action) { action_ = action; }

private:
  std::function<void(Item)> action_ = [](Item) {};
};

} // namespace dhe
