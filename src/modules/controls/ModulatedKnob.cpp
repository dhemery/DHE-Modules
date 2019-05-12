#include "modules/controls/ModulatedKnob.h"
#include "util/range.h"

namespace DHE {

ModulatedKnob::ModulatedKnob(Param &knob, Input &cvInput)
    : knob{knob}, cvInput{cvInput} {}

auto ModulatedKnob::rotation() -> float {
  auto constexpr attenuationRange = Range{-0.1f, 0.1f};
  auto constexpr avRotation = 1.f;

  auto const knobRotation = knob.getValue();
  auto const controlVoltage = cvInput.getVoltage();

  auto const attenuation = attenuationRange.scale(avRotation);

  return knobRotation + attenuation * controlVoltage;
};

} // namespace DHE
