#pragma once

#include "widgets/ControlWidgets.h"

#include <app/SvgSwitch.hpp>
#include <event.hpp>
#include <functional>
#include <utility>

namespace dhe {
namespace func {

  class MultiplicationRangeStepper : public Toggle {
  public:
    MultiplicationRangeStepper(std::string const &moduleSlug, rack::engine::Module *module, float x, float y,
                               int index) :
        Toggle("stepper-mult", 4, moduleSlug, module, x, y, index) {
      visible = false;
    }
  };

  class AdditionRangeStepper : public Toggle {
  public:
    AdditionRangeStepper(std::string const &moduleSlug, rack::engine::Module *module, float x, float y, int index) :
        Toggle("stepper-add", 4, moduleSlug, module, x, y, index) {}
  };

  class OperatorSwitch : public Toggle {
  public:
    OperatorSwitch(std::function<void(bool)> onOperatorChange, std::string const &moduleSlug,
                   rack::engine::Module *module, float x, float y, int index) :
        Toggle{2, moduleSlug, module, x, y, index}, operatorChangedTo{std::move(onOperatorChange)} {}

    void onChange(const rack::event::Change &e) override {
      rack::app::SvgSwitch::onChange(e);
      auto isMultiply = this->paramQuantity->getValue() > 0.5;
      operatorChangedTo(isMultiply);
    }

  private:
    std::function<void(bool)> operatorChangedTo = [](bool /*unused*/) {};
  };
} // namespace func

} // namespace dhe
