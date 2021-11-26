#pragma once

#include "rack.hpp"

#include <functional>

namespace dhe {

template <typename V = int>
struct SwitchQuantity : public rack::engine::SwitchQuantity {
  using Action = std::function<void(V)>;

  void setValue(float value) override {
    rack::engine::SwitchQuantity::setValue(value);
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
