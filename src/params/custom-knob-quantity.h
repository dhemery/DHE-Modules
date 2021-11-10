#pragma once

#include "rack.hpp"

namespace dhe {
template <typename TCustom>
struct CustomKnobQuantity : public rack::engine::ParamQuantity {
  using Converter = typename TCustom::ConverterType;

  auto getDisplayValue() -> float override {
    return converter_.display_value(getValue());
  }

  void setDisplayValue(float seconds) override {
    setValue(converter_.value(seconds));
  }

  auto converter() -> Converter & { return converter_; }

private:
  Converter converter_{};
};

} // namespace dhe
