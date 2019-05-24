#include "modules/Module.h"

#include "util/range.h"

namespace dhe {

constexpr auto avRange = Range{-1.f, 1.f};

auto cvOffset(float bipolarVoltage) -> float {
  static constexpr auto cvToOffset = 0.1f;
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

void Module::configKnob(int index, const std::string &name) { configParam(index, 0.f, 1.f, 0.5f, name); }

void Module::configCvGain(int index, const std::string &target) {
  configParam(index, 0.f, 1.f, 0.5f, target + " CV gain", "%", 0.f, 200.f, -100.f);
}

void Module::configGain(int index, const std::string &target) {
  configParam(index, 0.f, 1.f, 0.5f, target + " gain", "%", 0.f, 200.f);
}

void Module::configSignalRange(int index, const std::string &target, bool uni) {
  const auto initialValue = uni ? 1.f : 0.f;
  configParam(index, 0.f, 1.f, initialValue, target + " range");
}
} // namespace dhe
