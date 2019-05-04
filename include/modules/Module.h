#pragma once

#include "engine/Module.hpp"

namespace DHE {
class Module : public rack::engine::Module {
public:
  auto modulated(int knob_param, int cv_input) const -> float;

  auto modulated(int knob_param, int cv_input, int av_param) const -> float;

protected:
  void configKnob(int index, const std::string& name);
  void configCvGain(int index, const std::string &target);
  void configGain(int index, const std::string& target);
  void configSignalRange(int index, const std::string& target, bool uni = true);
};
} // namespace DHE
