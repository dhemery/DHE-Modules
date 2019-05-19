#pragma once

#include <engine/Param.hpp>
#include <engine/Port.hpp>

namespace DHE {

using Input = rack::engine::Input;
using Param = rack::engine::Param;
using Output = rack::engine::Output;

class ConstantInput : public Input {
public:
  explicit ConstantInput(float voltage) noexcept { setVoltage(voltage); }
};

class ConstantParam : public Param {
public:
  explicit ConstantParam(float value) noexcept { setValue(value); }
};

extern ConstantInput constant0VoltageInput;
extern ConstantParam constantFullyRotatedKnobParam;

} // namespace DHE
