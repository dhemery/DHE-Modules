#include "modules/controls/ModulatedKnob.h"
#include "util/range.h"

namespace DHE {

auto ModulatedKnob::rotation() const -> float {
  auto constexpr attenuationRange = Range{-0.1f, 0.1f};

  auto const knobRotation = knob.getValue();
  auto const controlVoltage = cvInput.getVoltage();
  auto const avRotation = avParam.getValue();

  auto const attenuation = attenuationRange.scale(avRotation);

  return knobRotation + attenuation * controlVoltage;
};

} // namespace DHE
