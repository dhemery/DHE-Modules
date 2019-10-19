#pragma once

#include "CurveSequencer.h"
#include "Sequence.h"
#include "panels/Panel.h"

#include <string>

namespace dhe {

template <typename P> class GenerateModeStepper : public Toggle<P, curve_sequencer::generateModeCount> {
public:
  GenerateModeStepper() : Toggle<P, curve_sequencer::generateModeCount>("stepper-generate") {}
};

template <typename P> class SustainModeStepper : public Toggle<P, curve_sequencer::sustainModeCount> {
public:
  SustainModeStepper() : Toggle<P, curve_sequencer::sustainModeCount>("stepper-sustain") {}
};

template <typename P> class StartMarker : public rack::widget::SvgWidget {
public:
  StartMarker() { setSvg(P::svg("marker-start")); }
};

template <typename P> class EndMarker : public rack::widget::SvgWidget {
public:
  EndMarker() { setSvg(P::svg("marker-end")); }
};

template <int N> class CurveSequencerPanel : public Panel<CurveSequencerPanel<N>> {
public:
  static std::string const moduleSlug;
  static auto constexpr stepWidth = 2.25F;
  static auto constexpr sequenceControlsWidth = 13.F;
  static auto constexpr hp = static_cast<int>(sequenceControlsWidth + N * stepWidth);

  CurveSequencerPanel(CurveSequencer<N> *sequencer) : Panel<CurveSequencerPanel<N>>(sequencer, hp) {
    auto const portRadius = 4.2F;
    auto const buttonRadius = 3.F;
    auto const lightRadius = 1.088F;

    auto const left = hp2mm(2.F);
    auto const right = hp2mm(hp - 2.F);
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

    this->input(moduleInputsX, runY, CurveSequencer<N>::RunInput);
    this->template button<ToggleButton>(moduleParamsX, runY, CurveSequencer<N>::RunButton);

    this->input(moduleInputsX, gateY, CurveSequencer<N>::GateInput);
    this->template button(moduleParamsX, gateY, CurveSequencer<N>::GateButton);

    this->input(moduleInputsX, resetY, CurveSequencer<N>::ResetInput);
    this->template button(moduleParamsX, resetY, CurveSequencer<N>::ResetButton);

    this->input(moduleInputsX, startY, CurveSequencer<N>::StartCVInput);
    auto *startKnob = this->template knob<SmallKnob>(moduleParamsX, startY, CurveSequencer<N>::StartKnob);
    startKnob->snap = true;

    this->input(moduleInputsX, stepsY, CurveSequencer<N>::StepsCVInput);
    auto *stepsKnob = this->template knob<SmallKnob>(moduleParamsX, stepsY, CurveSequencer<N>::StepsKnob);
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

    for (float step = 0; step < N; step++) {
      auto const x = stepX + stepDx * (float) step;
      this->light(x - activeLightXOffset, activeY, CurveSequencer<N>::GeneratingLights + step);
      this->template light<rack::componentlibrary::RedLight>(x + activeLightXOffset, activeY,
                                                             CurveSequencer<N>::SustainingLights + step);

      this->template toggle<GenerateModeStepper>(x, generatingModeY, CurveSequencer<N>::GenerateModeSwitches + step);
      this->template toggle<SustainModeStepper>(x, sustainingModeY, CurveSequencer<N>::SustainModeSwitches + step);

      this->template knob<SmallKnob>(x, levelY, CurveSequencer<N>::LevelKnobs + step);

      this->template toggle<2>(x, shapeY, CurveSequencer<N>::ShapeSwitches + step);
      this->template knob<SmallKnob>(x, curveY, CurveSequencer<N>::CurveKnobs + step);

      this->template knob<SmallKnob>(x, durationY, CurveSequencer<N>::DurationKnobs + step);

      this->template button<ToggleButton>(x, enabledButtonY, CurveSequencer<N>::EnabledButtons + step);
      this->input(x, enabledPortY, CurveSequencer<N>::EnabledInputs + step);
    }

    auto const moduleOutputsX = right;
    auto const outY = bottom - portRadius - 1.F;

    this->template toggle<2>(moduleOutputsX, levelY, CurveSequencer<N>::LevelRangeSwitch);
    this->template toggle<3>(moduleOutputsX, durationY, CurveSequencer<N>::DurationRangeSwitch);
    this->output(moduleOutputsX, outY, CurveSequencer<N>::OutOutput);

    this->addChild(rack::createWidgetCentered<StartMarker<CurveSequencerPanel<N>>>(
        rack::app::mm2px({stepX - activeLightXOffset, activeY})));
    this->addChild(rack::createWidgetCentered<EndMarker<CurveSequencerPanel<N>>>(
        rack::app::mm2px({stepX + activeLightXOffset, activeY})));
  }
}; // namespace dhe

template <int N> const std::string CurveSequencerPanel<N>::moduleSlug = "curve-sequencer-" + std::to_string(N);
} // namespace dhe
