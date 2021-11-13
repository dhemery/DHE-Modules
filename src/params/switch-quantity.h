#pragma once

#include "rack.hpp"

#include <functional>

namespace dhe {

template <typename V = int>
struct SwitchQuantity : public rack::engine::SwitchQuantity {
  using Action = std::function<void(V)>;

  void setValue(float value) override {
    rack::engine::SwitchQuantity::setValue(value);
    auto const v = static_cast<V>(value);
    action_(v);
  }

  void on_change(Action const &action) { action_ = action; }

private:
  Action action_ = [](V) {};
};

} // namespace dhe
