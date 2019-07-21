#include "panels/CurveSequencer16Panel.h"

namespace dhe {

template <typename P> class ShapeStepper : public Toggle<P, 2> {
public:
  ShapeStepper() : Toggle<P, 2>("stepper-shape") {}
};

template <typename P> class ModeStepper : public Toggle<P, 6> {
public:
  ModeStepper() : Toggle<P, 6>("stepper-mode") {}
};

CurveSequencer16Panel::CurveSequencer16Panel(CurveSequencer16 *sequencer) : Panel{sequencer, hp} {
  auto const left = hp2mm(2.F);
  auto const moduleInputsX = left;
  auto const moduleParamsX = left + hp2mm(2.F);
  auto const moduleOutputsX = width() - hp2mm(2.F);

  auto const StepX = hp2mm(10.F);
  auto const stepDx = hp2mm(2.25F);

  auto const enabledButtonY = hp2mm(4.F);
  auto const enabledPortY = enabledButtonY + hp2mm(1.6F);
  auto const enabledLabelY = (enabledButtonY + enabledPortY) / 2.F;
  auto const modeY = hp2mm(8.7F);
  auto const levelY = hp2mm(11.2F);
  auto const shapeY = hp2mm(14.F);
  auto const curveY = shapeY + hp2mm(1.6F);
  auto const durationY = hp2mm(18.5F);
  auto const eosY = hp2mm(22.F);

  auto const top = enabledLabelY;
  auto const bottom = eosY;

  auto const inputDy = (bottom - top) / 5;

  auto const runY = top + 0.F * inputDy;
  auto const gateY = top + 1.F * inputDy;
  auto const resetY = top + 2.F * inputDy;
  auto const loopY = top + 3.F * inputDy;
  auto const startY = top + 4.F * inputDy;
  auto const stepsY = top + 5.F * inputDy;

  input(moduleInputsX, runY, CurveSequencer16::RunInput);
  button<ToggleButton>(moduleParamsX, runY, CurveSequencer16::RunButton);

  input(moduleInputsX, gateY, CurveSequencer16::GateInput);
  button(moduleParamsX, gateY, CurveSequencer16::GateButton);

  input(moduleInputsX, resetY, CurveSequencer16::ResetInput);
  button(moduleParamsX, resetY, CurveSequencer16::ResetButton);

  input(moduleInputsX, loopY, CurveSequencer16::LoopInput);
  button<ToggleButton>(moduleParamsX, loopY, CurveSequencer16::LoopButton);

  input(moduleInputsX, startY, CurveSequencer16::StartCVInput);
  knob<SmallKnob>(moduleParamsX, startY, CurveSequencer16::StartKnob);

  input(moduleInputsX, stepsY, CurveSequencer16::StepsCVInput);
  knob<SmallKnob>(moduleParamsX, stepsY, CurveSequencer16::StepsKnob);

  toggle<2>(moduleOutputsX, levelY, CurveSequencer16::LevelRangeSwitch);
  toggle<3>(moduleOutputsX, durationY, CurveSequencer16::DurationRangeSwitch);

  output(moduleOutputsX, enabledPortY, CurveSequencer16::OutOutput);
  output(moduleOutputsX, eosY, CurveSequencer16::EOCOutput);

  for (int step = 0; step < CurveSequencer16::numberOfSteps; step++) {
    auto const x = StepX + step * stepDx;
    button<ToggleButton>(x, enabledButtonY, CurveSequencer16::EnabledButtons + step);
    input(x, enabledPortY, CurveSequencer16::EnabledInputs + step);

    toggle<ShapeStepper>(x, shapeY, CurveSequencer16::ShapeSwitches + step);

    knob<SmallKnob>(x, curveY, CurveSequencer16::CurveKnobs + step);
    knob<SmallKnob>(x, levelY, CurveSequencer16::LevelKnobs + step);
    knob<SmallKnob>(x, durationY, CurveSequencer16::DurationKnobs + step);

    toggle<ModeStepper>(x, modeY, CurveSequencer16::ModeSwitches + step);

    output(x, eosY, CurveSequencer16::EosOutputs + step);
  }
}
} // namespace dhe
