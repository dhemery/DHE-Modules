#pragma once

#include "engine/Module.hpp"

namespace dhe {
class Module : public rack::engine::Module {
public:
  auto modulated(int knob_param, int cv_input) -> float;
  auto modulated(int knob_param, int cv_input, int av_param) -> float;

protected:
  void configKnob(int index, const std::string &name);
  void configCvGain(int index, const std::string &target);
  void configGain(int index, const std::string &target);
  void configSignalRange(int index, const std::string &target, bool uni = true);
};
} // namespace dhe
