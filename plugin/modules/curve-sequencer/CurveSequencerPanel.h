#pragma once

#include "CurveSequencerControls.h"
#include "CurveSequencerModule.h"
#include "widgets/Panel.h"

#include <string>

namespace dhe {

namespace curve_sequencer {

  template <typename P> class GenerateModeStepper : public Toggle<P, generateModeCount> {
  public:
    GenerateModeStepper() : Toggle<P, generateModeCount>("stepper-generate") {}
  };

  template <typename P> class SustainModeStepper : public Toggle<P, sustainModeCount> {
  public:
    SustainModeStepper() : Toggle<P, sustainModeCount>("stepper-sustain") {}
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

    CurveSequencerPanel(CurveSequencerModule<N> *module) : Panel<CurveSequencerPanel<N>>(module, hp) {
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

      this->input(moduleInputsX, runY, CurveSequencerControls<N>::RunInput);
      this->template button<ToggleButton>(moduleParamsX, runY, CurveSequencerControls<N>::RunButton);

      this->input(moduleInputsX, gateY, CurveSequencerControls<N>::GateInput);
      this->template button(moduleParamsX, gateY, CurveSequencerControls<N>::GateButton);

      this->input(moduleInputsX, resetY, CurveSequencerControls<N>::ResetInput);
      this->template button(moduleParamsX, resetY, CurveSequencerControls<N>::ResetButton);

      this->input(moduleInputsX, startY, CurveSequencerControls<N>::StartCVInput);
      auto *startKnob = this->template knob<SmallKnob>(moduleParamsX, startY, CurveSequencerControls<N>::StartKnob);
      startKnob->snap = true;

      this->input(moduleInputsX, stepsY, CurveSequencerControls<N>::StepsCVInput);
      auto *stepsKnob = this->template knob<SmallKnob>(moduleParamsX, stepsY, CurveSequencerControls<N>::StepsKnob);
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
        this->light(x - activeLightXOffset, activeY, CurveSequencerControls<N>::GeneratingLights + step);
        this->template light<rack::componentlibrary::RedLight>(x + activeLightXOffset, activeY,
                                                               CurveSequencerControls<N>::SustainingLights + step);

        this->template toggle<GenerateModeStepper>(x, generatingModeY,
                                                   CurveSequencerControls<N>::GenerateModeSwitches + step);
        this->template toggle<SustainModeStepper>(x, sustainingModeY,
                                                  CurveSequencerControls<N>::SustainModeSwitches + step);

        this->template knob<SmallKnob>(x, levelY, CurveSequencerControls<N>::LevelKnobs + step);

        this->template toggle<2>(x, shapeY, CurveSequencerControls<N>::ShapeSwitches + step);
        this->template knob<SmallKnob>(x, curveY, CurveSequencerControls<N>::CurveKnobs + step);

        this->template knob<SmallKnob>(x, durationY, CurveSequencerControls<N>::DurationKnobs + step);

        this->template button<ToggleButton>(x, enabledButtonY, CurveSequencerControls<N>::EnabledButtons + step);
        this->input(x, enabledPortY, CurveSequencerControls<N>::EnabledInputs + step);
      }

      auto const moduleOutputsX = right;
      auto const outY = bottom - portRadius - 1.F;

      this->template toggle<2>(moduleOutputsX, levelY, CurveSequencerControls<N>::LevelRangeSwitch);
      this->template toggle<3>(moduleOutputsX, durationY, CurveSequencerControls<N>::DurationRangeSwitch);
      this->output(moduleOutputsX, outY, CurveSequencerControls<N>::OutOutput);

      this->addChild(rack::createWidgetCentered<StartMarker<CurveSequencerPanel<N>>>(
          rack::app::mm2px({stepX - activeLightXOffset, activeY})));
      this->addChild(rack::createWidgetCentered<EndMarker<CurveSequencerPanel<N>>>(
          rack::app::mm2px({stepX + activeLightXOffset, activeY})));
    }
  }; // namespace dhe

  template <int N> const std::string CurveSequencerPanel<N>::moduleSlug = "curve-sequencer-" + std::to_string(N);
} // namespace curve_sequencer
} // namespace dhe
