#pragma once

#include "rack.hpp"

#include <functional>

namespace dhe {

template <typename TValue = float>
struct KnobQuantity : public rack::engine::ParamQuantity {
  using TAction = std::function<void(TValue)>;

  void setValue(float value) override {
    rack::engine::ParamQuantity::setValue(value);
    auto const v = static_cast<TValue>(value);
    action_(v);
  }

  void on_change(TAction const &action) { action_ = action; }

private:
  TAction action_ = [](TValue) {};
};

} // namespace dhe
