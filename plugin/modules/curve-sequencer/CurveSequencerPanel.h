#pragma once

#include "CurveSequencerControls.h"
#include "CurveSequencerModule.h"
#include "widgets/Panel.h"

#include <functional>
#include <string>

namespace dhe {

namespace curve_sequencer {
  auto constexpr stepX = hp2mm(10.F);
  auto constexpr stepDx = hp2mm(2.25F);

  template <typename P> class GenerateModeStepper : public Toggle<P, modeCount> {
  public:
    GenerateModeStepper() : Toggle<P, modeCount>("stepper-mode") {}
  };

  template <typename P> class SustainModeStepper : public Toggle<P, advanceConditionCount> {
  public:
    SustainModeStepper() : Toggle<P, advanceConditionCount>("stepper-advance") {}
  };

  template <typename P> class SelectionKnob : public SmallKnob<P> {
  public:
    void onChange(const rack::event::Change &e) override {
      Knob<P>::onChange(e);
      knobChangedTo(static_cast<int>(this->paramQuantity->getValue()));
    }

    void onKnobChange(const std::function<void(int)> &action) { knobChangedTo = action; }

  private:
    std::function<void(int)> knobChangedTo = [](int /*unused*/) {};
  };

  template <typename P> class StartMarker : public rack::widget::SvgWidget {
  public:
    StartMarker() { setSvg(P::svg("marker-start")); }
    void setSelectionStart(int step) {
      auto constexpr baseX = stepX - 2.F * lightDiameter;
      auto const x = baseX + stepDx * static_cast<float>(step);
      this->box.pos.x = mm2px(x);
    }
  };

  template <template <int> class P, int N> class EndMarker : public rack::widget::SvgWidget {
  public:
    EndMarker() { setSvg(P<N>::svg("marker-end")); }
    void setSelectionStart(int step) {
      this->selectionStart = step;
      move();
    }
    void setSelectionLength(int length) {
      this->selectionLength = length;
      move();
    }

  private:
    void move() {
      auto const selectionEnd = (selectionStart + selectionLength - 1) & stepMask;
      auto const x = stepX + stepDx * static_cast<float>(selectionEnd);
      this->box.pos.x = mm2px(x);
    }

    int selectionStart{};
    int selectionLength{};
    int const stepMask = N - 1;
  }; // namespace curve_sequencer

  template <int N> class CurveSequencerPanel : public Panel<CurveSequencerPanel<N>> {
    using Controls = CurveSequencerControls<N>;

  public:
    static std::string const moduleSlug;
    static auto constexpr stepWidth = 2.25F;
    static auto constexpr sequenceControlsWidth = 13.F;
    static auto constexpr hp = static_cast<int>(sequenceControlsWidth + N * stepWidth);

    CurveSequencerPanel(CurveSequencerModule<N> *module) : Panel<CurveSequencerPanel<N>>(module, hp) {
      auto constexpr left = hp2mm(2.F);
      auto constexpr right = hp2mm(hp - 2.F);
      auto constexpr top = hp2mm(4.F);
      auto constexpr bottom = hp2mm(23);

      auto constexpr inputTop = top + hp2mm(2.75);
      auto constexpr inputBottom = bottom - portRadius - 1.F;
      auto constexpr inputDy = (inputBottom - inputTop) / 4.F;

      auto constexpr runY = inputTop + 0.F * inputDy;
      auto constexpr loopY = inputTop + 1.F * inputDy;
      auto constexpr selectionY = inputTop + 2.F * inputDy;
      auto constexpr gateY = inputTop + 3.F * inputDy;
      auto constexpr resetY = inputTop + 4.F * inputDy;

      this->input(left, runY, Controls::RunInput);
      this->template button<ToggleButton>(left + buttonPortDistance, runY, Controls::RunButton);

      this->input(left, loopY, Controls::LoopInput);
      this->template button<ToggleButton>(left + buttonPortDistance, loopY, Controls::LoopButton);

      auto *selectionStartKnob = this->template knob<SelectionKnob>(left, selectionY, Controls::SelectionStartKnob);
      selectionStartKnob->snap = true;

      auto constexpr selectionLengthX = left + hp2mm(2.F);

      auto *selectionLengthKnob
          = this->template knob<SelectionKnob>(selectionLengthX, selectionY, Controls::SelectionLengthKnob);
      selectionLengthKnob->snap = true;

      this->input(left, gateY, Controls::GateInput);
      this->template button(left + buttonPortDistance, gateY, Controls::GateButton);

      this->input(left, resetY, Controls::ResetInput);
      this->template button(left + buttonPortDistance, resetY, Controls::ResetButton);

      auto constexpr activeY = top + lightRadius;
      auto constexpr generatingModeY = top + hp2mm(2.25F);
      auto constexpr sustainingModeY = top + hp2mm(4.5F);
      auto constexpr levelY = top + hp2mm(6.75F);
      auto constexpr shapeY = top + hp2mm(9.25F);
      auto constexpr curveY = top + hp2mm(11.75F);
      auto constexpr durationY = top + hp2mm(14.25F);
      auto constexpr enabledPortY = bottom - portRadius;
      auto constexpr enabledButtonY = enabledPortY - portRadius - buttonRadius - 1.F;

      for (float step = 0; step < N; step++) {
        auto const x = stepX + stepDx * (float) step;
        this->template light<rack::componentlibrary::GreenRedLight>(x, activeY, Controls::ProgressLights + step + step);

        this->template toggle<GenerateModeStepper>(x, generatingModeY, Controls::ModeSwitches + step);
        this->template toggle<SustainModeStepper>(x, sustainingModeY, Controls::ConditionSwitches + step);

        this->template knob<SmallKnob>(x, levelY, Controls::LevelKnobs + step);

        this->template toggle<2>(x, shapeY, Controls::ShapeSwitches + step);
        this->template knob<SmallKnob>(x, curveY, Controls::CurveKnobs + step);

        this->template knob<SmallKnob>(x, durationY, Controls::DurationKnobs + step);

        this->template button<ToggleButton>(x, enabledButtonY, Controls::EnabledButtons + step);
        this->input(x, enabledPortY, Controls::EnabledInputs + step);
      }

      auto constexpr outY = bottom - portRadius - 1.F;
      auto constexpr eosY = top + hp2mm(2.75);

      this->input(right, eosY, Controls::CurveSequencerInput);

      this->template toggle<2>(right, levelY, Controls::LevelRangeSwitch);
      this->template toggle<3>(right, durationY, Controls::DurationRangeSwitch);
      this->output(right, outY, Controls::CurveSequencerOutput);

      auto *startMarker = rack::createWidgetCentered<StartMarker<CurveSequencerPanel<N>>>(mm2px(0.F, activeY));
      this->addChild(startMarker);

      auto *endMarker = rack::createWidgetCentered<EndMarker<CurveSequencerPanel, N>>(mm2px(0.F, activeY));
      this->addChild(endMarker);

      selectionStartKnob->onKnobChange([startMarker, endMarker](int step) {
        startMarker->setSelectionStart(step);
        endMarker->setSelectionStart(step);
      });

      selectionLengthKnob->onKnobChange([endMarker](int length) { endMarker->setSelectionLength(length); });
    }
  }; // namespace dhe

  template <int N> const std::string CurveSequencerPanel<N>::moduleSlug = "curve-sequencer-" + std::to_string(N);
} // namespace curve_sequencer
} // namespace dhe
