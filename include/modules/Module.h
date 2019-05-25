#pragma once

#include "engine/Module.hpp"

namespace dhe {
class Module : public rack::engine::Module {
public:
  auto modulated(int knobId, int cvId) -> float;
  auto modulated(int knobId, int cvId, int avId) -> float;

protected:
  void configKnob(int index, const std::string &name);
  void configGain(int index, const std::string &target);
};
} // namespace dhe
