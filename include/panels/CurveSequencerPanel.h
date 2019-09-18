#pragma once

#include "Panel.h"
#include "modules/CurveSequencerModule.h"

namespace dhe {

template <typename P> class GenerateModeStepper : public Toggle<P, 7> {
public:
  GenerateModeStepper() : Toggle<P, 7>("stepper-generate") {}
};

template <typename P> class SustainModeStepper : public Toggle<P, 6> {
public:
  SustainModeStepper() : Toggle<P, 6>("stepper-sustain") {}
};

template <typename P> class StartMarker : public rack::widget::SvgWidget {
public:
  StartMarker() { setSvg(P::svg("marker-start")); }
};

template <typename P> class EndMarker : public rack::widget::SvgWidget {
public:
  EndMarker() { setSvg(P::svg("marker-end")); }
};

template <int NS, int HP> class CurveSequencerPanel : public Panel<CurveSequencerPanel<NS, HP>> {
public:
  static constexpr auto moduleSlug = NS == 8 ? "curve-sequencer-8" : "curve-sequencer-16";
  static constexpr auto hp = HP;

  CurveSequencerPanel(CurveSequencerModule<NS> *sequencer) : Panel<CurveSequencerPanel<NS, HP>>(sequencer, HP) {
    auto const portRadius = 4.2F;
    auto const buttonRadius = 3.F;
    auto const lightRadius = 1.088F;

    auto const left = hp2mm(2.F);
    auto const right = hp2mm(HP - 2.F);
    auto const top = hp2mm(4.F);
    auto const bottom = hp2mm(23);

    auto const inputTop = top + 7.F;
    auto const inputBottom = bottom - 4.2F;
    auto const inputDy = (inputBottom - inputTop) / 4.F;

    auto const moduleInputsX = left;
    auto const moduleParamsX = left + hp2mm(2.F);

    auto const runY = inputTop + 0.F * inputDy;
    auto const gateY = inputTop + 1.F * inputDy;
    auto const resetY = inputTop + 2.F * inputDy;
    auto const startY = inputTop + 3.F * inputDy;
    auto const stepsY = inputTop + 4.F * inputDy;

    this->input(moduleInputsX, runY, CurveSequencerModule<NS>::RunInput);
    this->template button<ToggleButton>(moduleParamsX, runY, CurveSequencerModule<NS>::RunButton);

    this->input(moduleInputsX, gateY, CurveSequencerModule<NS>::GateInput);
    this->template button(moduleParamsX, gateY, CurveSequencerModule<NS>::GateButton);

    this->input(moduleInputsX, resetY, CurveSequencerModule<NS>::ResetInput);
    this->template button(moduleParamsX, resetY, CurveSequencerModule<NS>::ResetButton);

    this->input(moduleInputsX, startY, CurveSequencerModule<NS>::StartCVInput);
    auto *startKnob = this->template knob<SmallKnob>(moduleParamsX, startY, CurveSequencerModule<NS>::StartKnob);
    startKnob->snap = true;

    this->input(moduleInputsX, stepsY, CurveSequencerModule<NS>::StepsCVInput);
    auto *stepsKnob = this->template knob<SmallKnob>(moduleParamsX, stepsY, CurveSequencerModule<NS>::StepsKnob);
    stepsKnob->snap = true;

    auto const stepX = hp2mm(10.F);
    auto const stepDx = hp2mm(2.25F);

    auto const activeY = top + lightRadius;
    auto const generatingModeY = top + hp2mm(2.25F);
    auto const sustainingModeY = top + hp2mm(4.5F);
    auto const levelY = top + hp2mm(6.75F);
    auto const shapeY = top + hp2mm(9.25F);
    auto const curveY = top + hp2mm(11.75F);
    auto const durationY = top + hp2mm(14.25F);
    auto const enabledPortY = bottom - portRadius;
    auto const enabledButtonY = enabledPortY - portRadius - buttonRadius - 1.F;

    auto const activeLightXOffset = lightRadius * 2.F;

    for (int step = 0; step < NS; step++) {
      auto const x = stepX + step * stepDx;
      this->light(x - activeLightXOffset, activeY, CurveSequencerModule<NS>::GeneratingLights + step);
      this->template light<rack::componentlibrary::YellowLight>(x + activeLightXOffset, activeY,
                                                                CurveSequencerModule<NS>::SustainingLights + step);

      this->template toggle<GenerateModeStepper>(x, generatingModeY,
                                                 CurveSequencerModule<NS>::GenerateModeSwitches + step);
      this->template toggle<SustainModeStepper>(x, sustainingModeY,
                                                CurveSequencerModule<NS>::SustainModeSwitches + step);

      this->template knob<SmallKnob>(x, levelY, CurveSequencerModule<NS>::LevelKnobs + step);

      this->template toggle<2>(x, shapeY, CurveSequencerModule<NS>::ShapeSwitches + step);
      this->template knob<SmallKnob>(x, curveY, CurveSequencerModule<NS>::CurveKnobs + step);

      this->template knob<SmallKnob>(x, durationY, CurveSequencerModule<NS>::DurationKnobs + step);

      this->template button<ToggleButton>(x, enabledButtonY, CurveSequencerModule<NS>::EnabledButtons + step);
      this->input(x, enabledPortY, CurveSequencerModule<NS>::EnabledInputs + step);
    }

    auto const moduleOutputsX = right;
    auto const outY = bottom - portRadius - 1.F;

    this->template toggle<2>(moduleOutputsX, levelY, CurveSequencerModule<NS>::LevelRangeSwitch);
    this->template toggle<3>(moduleOutputsX, durationY, CurveSequencerModule<NS>::DurationRangeSwitch);
    this->output(moduleOutputsX, outY, CurveSequencerModule<NS>::OutOutput);

    this->addChild(rack::createWidgetCentered<StartMarker<CurveSequencerPanel<NS, HP>>>(
        rack::app::mm2px({stepX - activeLightXOffset, activeY})));
    this->addChild(rack::createWidgetCentered<EndMarker<CurveSequencerPanel<NS, HP>>>(
        rack::app::mm2px({stepX + activeLightXOffset, activeY})));
    //    this->addChild(rack::createWidgetCentered<EndMarker<CurveSequencerPanel<NS,HP>>>(rack::app::mm2px({stepX +
    //    stepDx * (NS-1.F), markerMiddle})));
  }
}; // namespace dhe
} // namespace dhe
