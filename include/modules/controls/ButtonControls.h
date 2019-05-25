#pragma once

#include <array>
#include <string>

namespace dhe {

namespace button {
  class ButtonParamQuantity : public rack::engine::ParamQuantity {
    std::array<std::string, 2> positionNames;

  public:
    void setPositionNames(std::array<std::string, 2> const &names) { positionNames = names; }
    auto getDisplayValueString() -> std::string override {
      auto const position = static_cast<int>(getValue());
      return positionNames[position];
    }
  };

  void config(rack::engine::Module *module, int switchId, std::string const &switchName,
              std::array<std::string, 2> const &positionNames, int initialPosition) {
    module->configParam<ButtonParamQuantity>(switchId, 0.F, 1.F, (float) initialPosition, switchName);
    auto knobParamQuantity = dynamic_cast<ButtonParamQuantity *>(module->paramQuantities[switchId]);
    knobParamQuantity->setPositionNames(positionNames);
  }
} // namespace button
} // namespace dhe
