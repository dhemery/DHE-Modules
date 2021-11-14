#pragma once

#include "rack.hpp"

namespace dhe {
template <typename T>
struct MappedKnobQuantity : public rack::engine::ParamQuantity {
  using Mapper = typename T::KnobMapper;

  auto getDisplayValue() -> float override {
    return map_.to_display(getValue());
  }

  void setDisplayValue(float scaled) override {
    setValue(map_.to_value(scaled));
  }

  auto mapper() -> Mapper & { return map_; }

private:
  Mapper map_{};
};

} // namespace dhe
