#pragma once

#include "rack.hpp"

#include <functional>

namespace dhe {

template <typename TValue = float>
struct KnobQuantity : public rack::engine::ParamQuantity {
  using Action = std::function<void(TValue)>;

  void setValue(float value) override {
    rack::engine::ParamQuantity::setValue(value);
    action_(static_cast<TValue>(value));
  }

  void on_change(Action const &action) { action_ = action; }

private:
  Action action_ = [](TValue) {};
};

} // namespace dhe
