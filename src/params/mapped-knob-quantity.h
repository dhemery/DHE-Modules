#pragma once

#include "rack.hpp"

namespace dhe {
template <typename TCustom>
struct MappedKnobQuantity : public rack::engine::ParamQuantity {
  using Mapper = typename TCustom::KnobMapper;

  auto getDisplayValue() -> float override {
    return map_.to_display_value(getValue());
  }

  void setDisplayValue(float display_value) override {
    setValue(map_.to_rotation(display_value));
  }

  auto mapper() -> Mapper & { return map_; }

private:
  Mapper map_{};
};

} // namespace dhe
