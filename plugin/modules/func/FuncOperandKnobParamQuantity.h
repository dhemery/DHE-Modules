#pragma once

namespace dhe {
namespace func {
  class FuncOperandKnobParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getLabel() -> std::string override { return currentOperandName(); }

    auto getDisplayValue() -> float override {
      auto const rotation = getValue();
      auto const operandRange = currentOperandRange();
      auto const operand = operandRange->scale(rotation);
      return operand;
    }

    void setDisplayValue(float operand) override {
      auto const operandRange = currentOperandRange();
      auto const rotation = operandRange->normalize(operand);
      setValue(rotation);
    }

    void configure(rack::engine::Param &newOperationToggle, rack::engine::Param &newMultiplicationRangeToggle,
                   rack::engine::Param &newAdditionRangeToggle) {
      operationToggle = &newOperationToggle;
      multiplicationRangeToggle = &newMultiplicationRangeToggle;
      additionRangeToggle = &newAdditionRangeToggle;
    }

    auto currentOperandRange() -> Range const * {
      return positionOf(*operationToggle) == 1 ? multiplicationRange() : offsetRange();
    }

    auto currentOperandName() const -> std::string const & {
      static auto operandNames = std::array<std::string, 2>{"Offset", "Multiplier"};
      return operandNames[positionOf(*operationToggle)];
    }

    auto multiplicationRange() -> Range const * { return multiplierRanges[positionOf(*multiplicationRangeToggle)]; }

    auto offsetRange() -> Range const * { return offsetRanges[positionOf(*additionRangeToggle)]; }

  private:
    rack::engine::Param *operationToggle;
    rack::engine::Param *multiplicationRangeToggle;
    rack::engine::Param *additionRangeToggle;
  };
} // namespace func
}