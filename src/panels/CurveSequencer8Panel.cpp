#include "panels/CurveSequencer8Panel.h"

namespace dhe {

template <typename P> class ShapeStepper : public Toggle<P, 2> {
public:
  ShapeStepper() : Toggle<P, 2>("stepper-shape") {}
};

template <typename P> class ModeStepper : public Toggle<P, 6> {
public:
  ModeStepper() : Toggle<P, 6>("stepper-mode") {}
};

CurveSequencer8Panel::CurveSequencer8Panel(CurveSequencer8 *sequencer) : Panel{sequencer, hp} {
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

  input(moduleInputsX, runY, CurveSequencer8::RunInput);
  button<ToggleButton>(moduleParamsX, runY, CurveSequencer8::RunButton);

  input(moduleInputsX, gateY, CurveSequencer8::GateInput);
  button(moduleParamsX, gateY, CurveSequencer8::GateButton);

  input(moduleInputsX, resetY, CurveSequencer8::ResetInput);
  button(moduleParamsX, resetY, CurveSequencer8::ResetButton);

  input(moduleInputsX, loopY, CurveSequencer8::LoopInput);
  button<ToggleButton>(moduleParamsX, loopY, CurveSequencer8::LoopButton);

  input(moduleInputsX, startY, CurveSequencer8::StartCVInput);
  knob<SmallKnob>(moduleParamsX, startY, CurveSequencer8::StartKnob);

  input(moduleInputsX, stepsY, CurveSequencer8::StepsCVInput);
  knob<SmallKnob>(moduleParamsX, stepsY, CurveSequencer8::StepsKnob);

  toggle<2>(moduleOutputsX, levelY, CurveSequencer8::LevelRangeSwitch);
  toggle<3>(moduleOutputsX, durationY, CurveSequencer8::DurationRangeSwitch);

  output(moduleOutputsX, enabledPortY, CurveSequencer8::OutOutput);
  output(moduleOutputsX, eosY, CurveSequencer8::EOCOutput);

  for (int step = 0; step < CurveSequencer8::numberOfSteps; step++) {
    auto const x = StepX + step * stepDx;
    button<ToggleButton>(x, enabledButtonY, CurveSequencer8::EnabledButtons + step);
    input(x, enabledPortY, CurveSequencer8::EnabledInputs + step);

    toggle<ShapeStepper>(x, shapeY, CurveSequencer8::ShapeSwitches + step);

    knob<SmallKnob>(x, curveY, CurveSequencer8::CurveKnobs + step);
    knob<SmallKnob>(x, levelY, CurveSequencer8::LevelKnobs + step);
    knob<SmallKnob>(x, durationY, CurveSequencer8::DurationKnobs + step);

    toggle<ModeStepper>(x, modeY, CurveSequencer8::ModeSwitches + step);

    output(x, eosY, CurveSequencer8::EosOutputs + step);
  }
}
} // namespace dhe
