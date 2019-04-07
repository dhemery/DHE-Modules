#pragma once

namespace DHE {
template <typename P> class MultiplicationRangeStepper : public Toggle<P, 4> {
public:
  MultiplicationRangeStepper() : Toggle<P, 4>("stepper-mult") {}
};

template <typename P> class AdditionRangeStepper : public Toggle<P, 4> {
public:
  AdditionRangeStepper() : Toggle<P, 4>("stepper-add") {}
};

} // namespace DHE
