#pragma once

#include <array>

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

#include "modules/controls/ModulatedKnob.h"
#include "util/range.h"

namespace DHE {

namespace Duration {
extern std::array<Range const *, 3> const ranges;

auto range(float switchPosition) -> Range const *;
auto range(Param &rangeSwitchParam) -> Range const *;

class Control : public ModulatedKnob {

public:
  explicit Control(Param &knob,
                   Param &rangeSwitch = constantMediumDurationRangeSwitch,
                   Input &cvInput = constant0VoltageInput,
                   Param &avParam = constantFullyRotatedKnobParam);

  auto seconds() -> float;

  static ConstantParam constantMediumDurationRangeSwitch;
private:
  Param &rangeSwitch;
};

class RangeSwitchParamQuantity : public rack::engine::ParamQuantity {
public:
  std::string getDisplayValueString() override;
};

class KnobParamQuantity : public rack::engine::ParamQuantity {
public:
  float getDisplayValue() override;
  void setDisplayValue(float displayValue) override;

  auto range() -> Range const *;

  Param *rangeSwitch;
};

void config(rack::engine::Module *module, int knobParamId,
            int rangeSwitchParamId);
void config(rack::engine::Module *module, int knobParamId);
} // namespace Duration

} // namespace DHE
