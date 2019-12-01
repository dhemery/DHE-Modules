#pragma once

#include "widgets/Toggles.h"

#include <app/SvgSwitch.hpp>
#include <event.hpp>
#include <functional>

namespace dhe {
namespace func {

  class MultiplicationRangeStepper : public Toggle<4> {
  public:
    MultiplicationRangeStepper() : Toggle<4>("stepper-mult") {}
  };

  class AdditionRangeStepper : public Toggle<4> {
  public:
    AdditionRangeStepper() : Toggle<4>("stepper-add") {}
  };

  class OperatorSwitch : public Toggle<2> {
  public:
    void onChange(const rack::event::Change &e) override {
      rack::app::SvgSwitch::onChange(e);
      auto isMultiply = this->paramQuantity->getValue() > 0.5;
      operatorChangedTo(isMultiply);
    }

    void onOperatorChange(const std::function<void(bool)> &action) { operatorChangedTo = action; }

  private:
    std::function<void(bool)> operatorChangedTo = [](bool /*unused*/) {};
  };
} // namespace func

} // namespace dhe
