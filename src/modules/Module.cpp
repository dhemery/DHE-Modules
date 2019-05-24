#include "modules/Module.h"

#include "util/range.h"

namespace dhe {

constexpr auto avRange = Range{-1.F, 1.F};

auto cvOffset(float bipolarVoltage) -> float {
  static constexpr auto cvToOffset = 0.1F;
  return bipolarVoltage * cvToOffset;
}

auto avMultiplier(float avAmount) -> float { return avRange.scale(avAmount); }

auto Module::modulated(int knobId, int cvId) -> float {
  auto rotation = params[knobId].getValue();
  auto cv = inputs[cvId].getVoltage();
  return rotation + cvOffset(cv);
}

auto Module::modulated(int knobId, int cvId, int avId) -> float {
  auto rotation = params[knobId].getValue();
  auto cv = inputs[cvId].getVoltage();
  auto av = params[avId].getValue();
  return rotation + avMultiplier(av) * cvOffset(cv);
}

void Module::configKnob(int index, const std::string &name) { configParam(index, 0.F, 1.F, 0.5F, name); }

void Module::configCvGain(int index, const std::string &target) {
  configParam(index, 0.F, 1.F, 0.5F, target + " CV gain", "%", 0.F, 200.F, -100.F);
}

void Module::configGain(int index, const std::string &target) {
  configParam(index, 0.F, 1.F, 0.5F, target + " gain", "%", 0.F, 200.F);
}

void Module::configSignalRange(int index, const std::string &target, bool uni) {
  const auto initialValue = uni ? 1.F : 0.F;
  configParam(index, 0.F, 1.F, initialValue, target + " range");
}
} // namespace dhe
