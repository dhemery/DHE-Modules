#pragma once

#include "rack.hpp"

namespace dhe {
template <typename TCustom>
struct MappedKnobQuantity : public rack::engine::ParamQuantity {
  using Mapper = typename TCustom::DisplayMapper;

  auto getDisplayValue() -> float override {
    return map_.to_display_value(getValue());
  }

  void setDisplayValue(float seconds) override {
    setValue(map_.to_value(seconds));
  }

  auto mapper() -> Mapper & { return map_; }

private:
  Mapper map_{};
};

} // namespace dhe
