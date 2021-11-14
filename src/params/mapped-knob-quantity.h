#pragma once

#include "rack.hpp"

namespace dhe {
template <typename T>
struct MappedKnobQuantity : public rack::engine::ParamQuantity {
  using MapType = typename T::KnobMap;

  auto getDisplayValue() -> float override {
    return map_.to_display(getValue());
  }

  void setDisplayValue(float scaled) override {
    setValue(map_.to_value(scaled));
  }

  auto mapper() -> MapType & { return map_; }

private:
  MapType map_{};
};

} // namespace dhe
