#pragma once

#include "rack.hpp"

#include <functional>

namespace dhe {

template <typename TItems>
struct PickerSwitchQuantity : public rack::engine::SwitchQuantity {
  using ItemType = typename TItems::ItemType;
  using Action = std::function<void(ItemType)>;

  void setValue(float f) override {
    rack::engine::SwitchQuantity::setValue(f);
    auto const selection = static_cast<int>(f);
    action_(TItems::select(selection));
  }

  void on_change(Action const &action) { action_ = action; }

private:
  std::function<void(ItemType)> action_ = [](ItemType) {};
};

} // namespace dhe
