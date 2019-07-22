#pragma once

#include "Panel.h"
#include "modules/CurveSequencer.h"

namespace dhe {

template <typename P> class ShapeStepper : public Toggle<P, 2> {
public:
  ShapeStepper() : Toggle<P, 2>("stepper-shape") {}
};

template <typename P> class ModeStepper : public Toggle<P, 6> {
public:
  ModeStepper() : Toggle<P, 6>("stepper-mode") {}
};

template <int NS, int HP> class CurveSequencerPanel : public Panel<CurveSequencerPanel<NS, HP>> {
public:
  static constexpr auto moduleSlug = NS == 8 ? "curve-sequencer-8" : "curve-sequencer-16";
  static constexpr auto hp = HP;

  CurveSequencerPanel(CurveSequencer<NS> *sequencer) : Panel<CurveSequencerPanel<NS, HP>>(sequencer, HP) {
    auto const left = hp2mm(2.F);
    auto const moduleInputsX = left;
    auto const moduleParamsX = left + hp2mm(2.F);
    auto const moduleOutputsX = hp2mm(HP - 2.F);

    auto const stepX = hp2mm(10.F);
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

    this->input(moduleInputsX, runY, CurveSequencer<NS>::RunInput);
    this->template button<ToggleButton>(moduleParamsX, runY, CurveSequencer<NS>::RunButton);

    this->input(moduleInputsX, gateY, CurveSequencer<NS>::GateInput);
    this->template button(moduleParamsX, gateY, CurveSequencer<NS>::GateButton);

    this->input(moduleInputsX, resetY, CurveSequencer<NS>::ResetInput);
    this->template button(moduleParamsX, resetY, CurveSequencer<NS>::ResetButton);

    this->input(moduleInputsX, loopY, CurveSequencer<NS>::LoopInput);
    this->template button<ToggleButton>(moduleParamsX, loopY, CurveSequencer<NS>::LoopButton);

    this->input(moduleInputsX, startY, CurveSequencer<NS>::StartCVInput);
    this->template knob<SmallKnob>(moduleParamsX, startY, CurveSequencer<NS>::StartKnob);

    this->input(moduleInputsX, stepsY, CurveSequencer<NS>::StepsCVInput);
    this->template knob<SmallKnob>(moduleParamsX, stepsY, CurveSequencer<NS>::StepsKnob);

    this->template toggle<2>(moduleOutputsX, levelY, CurveSequencer<NS>::LevelRangeSwitch);
    this->template toggle<3>(moduleOutputsX, durationY, CurveSequencer<NS>::DurationRangeSwitch);

    this->output(moduleOutputsX, enabledPortY, CurveSequencer<NS>::OutOutput);
    this->output(moduleOutputsX, eosY, CurveSequencer<NS>::EOCOutput);

    for (int step = 0; step < NS; step++) {
      auto const x = stepX + step * stepDx;
      this->template button<ToggleButton>(x, enabledButtonY, CurveSequencer<NS>::EnabledButtons + step);
      this->input(x, enabledPortY, CurveSequencer<NS>::EnabledInputs + step);

      this->template toggle<ShapeStepper>(x, shapeY, CurveSequencer<NS>::ShapeSwitches + step);

      this->template knob<SmallKnob>(x, curveY, CurveSequencer<NS>::CurveKnobs + step);
      this->template knob<SmallKnob>(x, levelY, CurveSequencer<NS>::LevelKnobs + step);
      this->template knob<SmallKnob>(x, durationY, CurveSequencer<NS>::DurationKnobs + step);

      this->template toggle<ModeStepper> (x, modeY, CurveSequencer<NS>::ModeSwitches + step);

      this->light(x, eosY, CurveSequencer<NS>::ActivityLights + step);
    }
  }
}; // namespace dhe
} // namespace dhe
