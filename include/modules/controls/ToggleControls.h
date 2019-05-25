#pragma once

namespace dhe {

namespace toggle {
  template <int N> class ToggleParamQuantity : public rack::engine::ParamQuantity {
    std::array<std::string, N> positionNames;

  public:
    void setPositionNames(std::array<std::string, N> const &names) { positionNames = names; }
    auto getDisplayValueString() -> std::string override {
      auto const position = static_cast<int>(getValue());
      return positionNames[position];
    }
  };

  template <int N>
  void config(rack::engine::Module *module, int switchId, std::string const &switchName,
              std::array<std::string, N> const &positionNames, int initialPosition = 0) {
    module->configParam<ToggleParamQuantity<N>>(switchId, 0.F, (float) (N - 1), (float) initialPosition, switchName);
    auto knobParamQuantity = dynamic_cast<ToggleParamQuantity<N> *>(module->paramQuantities[switchId]);
    knobParamQuantity->setPositionNames(positionNames);
  }
} // namespace toggle
} // namespace dhe
