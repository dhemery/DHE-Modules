#pragma once

#include "rack.hpp"

namespace dhe {
template <typename TCustom>
struct MappedKnobQuantity : public rack::engine::ParamQuantity {
  using Mapper = typename TCustom::KnobMapper;

  auto getDisplayValue() -> float override { return map_.scale(getValue()); }

  void setDisplayValue(float scaled) override {
    setValue(map_.normalize(scaled));
  }

  auto mapper() -> Mapper & { return map_; }

private:
  Mapper map_{};
};

} // namespace dhe
