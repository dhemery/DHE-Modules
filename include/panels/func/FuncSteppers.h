#pragma once

#include <functional>

namespace dhe {
template <typename P> class MultiplicationRangeStepper : public Toggle<P, 4> {
public:
  MultiplicationRangeStepper() : Toggle<P, 4>("stepper-mult") {}
};

template <typename P> class AdditionRangeStepper : public Toggle<P, 4> {
public:
  AdditionRangeStepper() : Toggle<P, 4>("stepper-add") {}
};

template <typename P> class OperatorSwitch : public Toggle<P, 2> {
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

} // namespace dhe
