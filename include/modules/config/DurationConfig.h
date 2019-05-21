#pragma once

#include <array>
#include <functional>

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

#include "util/range.h"

namespace DHE {

namespace Duration {

class RangeParamQuantity : public rack::engine::ParamQuantity {
public:
  std::string getDisplayValueString() override;

  std::function<Range const *()> getRange;
};

class KnobParamQuantity : public rack::engine::ParamQuantity {
public:
  float getDisplayValue() override;
  void setDisplayValue(float displayValue) override;

  std::function<Range const *()> getRange;
};

void config(rack::engine::Module *module, int knobParamId,
            std::function<Range const *()> const &getRange);

void config(rack::engine::Module *module, int knobParamId,
            int rangeSwitchParamId,
            std::function<Range const *()> const &getRange);
} // namespace Duration

} // namespace DHE
