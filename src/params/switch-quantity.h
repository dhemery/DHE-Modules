#pragma once

#include "rack.hpp"

#include <functional>

namespace dhe {

template <typename TValue = int>
struct SwitchQuantity : public rack::engine::SwitchQuantity {
  using Action = std::function<void(TValue)>;

  void setValue(float value) override {
    rack::engine::SwitchQuantity::setValue(value);
    auto const v = static_cast<TValue>(value);
    action_(v);
  }

  void on_change(Action const &action) { action_ = action; }

private:
  Action action_ = [](TValue) {};
};

} // namespace dhe
