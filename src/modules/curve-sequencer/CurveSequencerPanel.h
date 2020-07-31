#pragma once

#include "CurveSequencerControls.h"
#include "widgets/ControlWidgets.h"
#include "widgets/PickList.h"
#include "widgets/Screws.h"

#include <app/ModuleWidget.hpp>
#include <componentlibrary.hpp>
#include <functional>
#include <string>
#include <utility>

namespace dhe {

namespace curve_sequencer {
  auto constexpr stepX = hp2mm(10.F);
  auto constexpr stepDx = hp2mm(2.25F);

  using ProgressLight = rack::componentlibrary::SmallLight<rack::componentlibrary::GreenRedLight>;

  class SelectionKnob : public Knob {
  public:
    SelectionKnob(std::function<void(int)> action, std::string const &moduleSvgDir, rack::engine::Module *module,
                  float x, float y, int index) :
        Knob{moduleSvgDir, "knob-small", module, x, y, index}, knobChangedTo{std::move(action)} {
      snap = true;
    }

    void onChange(const rack::event::Change &e) override {
      Knob::onChange(e);
      knobChangedTo(static_cast<int>(this->paramQuantity->getValue()));
    }

  private:
    std::function<void(int)> knobChangedTo;
  };

  class StartMarker : public rack::widget::SvgWidget {
  public:
    StartMarker(std::string const &moduleSvgDir, float x, float y) {
      setSvg(controlSvg(moduleSvgDir, "marker-start"));
      positionCentered(this, x, y);
    }

    void setSelectionStart(int step) {
      auto constexpr baseX = stepX - 2.F * lightDiameter;
      auto const x = baseX + stepDx * static_cast<float>(step);
      this->box.pos.x = mm2px(x);
    }
  };

  template <int N> class EndMarker : public rack::widget::SvgWidget {
  public:
    EndMarker(std::string const &moduleSvgDir, float x, float y) {
      setSvg(controlSvg(moduleSvgDir, "marker-end"));
      positionCentered(this, x, y);
    }

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

  template <int N> class CurveSequencerPanel : public rack::app::ModuleWidget {
    using Controls = CurveSequencerControls<N>;

  public:
    CurveSequencerPanel(rack::engine::Module *module) {
      auto const slug = std::string{"curve-sequencer-"} + std::to_string(N);
      auto constexpr stepWidth = 2.25F;
      auto constexpr sequenceControlsWidth = 13.F;
      auto constexpr hp = static_cast<int>(sequenceControlsWidth + N * stepWidth);

      setModule(module);
      setPanel(backgroundSvg(slug));
      installScrews(this, hp);

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

      auto constexpr activeY = top + lightRadius;

      addInput(Jack::input(slug, module, left, runY, Controls::RunInput));
      addParam(Toggle::button(slug, module, left + buttonPortDistance, runY, Controls::RunButton));

      addInput(Jack::input(slug, module, left, loopY, Controls::LoopInput));
      addParam(Toggle::button(slug, module, left + buttonPortDistance, loopY, Controls::LoopButton));

      auto *startMarker = new StartMarker(slug, 0.F, activeY);
      addChild(startMarker);

      auto *endMarker = new EndMarker<N>(slug, 0.F, activeY);
      addChild(endMarker);

      auto const onSelectionStartChange = [startMarker, endMarker](int step) {
        startMarker->setSelectionStart(step);
        endMarker->setSelectionStart(step);
      };
      addParam(new SelectionKnob(onSelectionStartChange, slug, module, left, selectionY, Controls::SelectionStartKnob));

      auto const onSelectionEndChange = [endMarker](int length) { endMarker->setSelectionLength(length); };
      auto constexpr selectionLengthX = left + hp2mm(2.F);
      addParam(new SelectionKnob(onSelectionEndChange, slug, module, selectionLengthX, selectionY,
                                 Controls::SelectionLengthKnob));

      addInput(Jack::input(slug, module, left, gateY, Controls::GateInput));
      addParam(Button::momentary(slug, module, left + buttonPortDistance, gateY, Controls::GateButton));

      addInput(Jack::input(slug, module, left, resetY, Controls::ResetInput));
      addParam(Button::momentary(slug, module, left + buttonPortDistance, resetY, Controls::ResetButton));

      auto constexpr generateModeY = top + hp2mm(2.25F);
      auto constexpr advanceModeY = top + hp2mm(4.5F);
      auto constexpr levelY = top + hp2mm(6.75F);
      auto constexpr shapeY = top + hp2mm(9.25F);
      auto constexpr curveY = top + hp2mm(11.75F);
      auto constexpr durationY = top + hp2mm(14.25F);
      auto constexpr enabledPortY = bottom - portRadius;
      auto constexpr enabledButtonY = enabledPortY - portRadius - buttonRadius - 1.F;

      auto const generateModeLabels = std::vector<std::string>{"curve", "hold", "sustain", "input", "chase", "level"};
      auto const advanceModeLabels = std::vector<std::string>{"time", "rise", "fall", "edge", "high", "low"};

      // Gather popup menus to add after all other controls, so the menus display on top of the other controls
      auto popupMenus = std::vector<rack::widget::Widget *>{};

      for (auto step = 0; step < N; step++) {
        auto const x = stepX + stepDx * (float) step;
        addChild(rack::createLightCentered<ProgressLight>(mm2px(x, activeY), module,
                                                          Controls::ProgressLights + step + step));

        auto *generateModeButton = picklist::button(slug, "generate-mode", generateModeLabels.size(), module, x,
                                                    generateModeY, Controls::ModeSwitches + step);
        addParam(generateModeButton);
        popupMenus.push_back(generateModeButton->menu());

        auto *advanceModeButton = picklist::button(slug, "advance-mode", advanceModeLabels.size(), module, x,
                                                   advanceModeY, Controls::ConditionSwitches + step);
        addParam(advanceModeButton);
        popupMenus.push_back(advanceModeButton->menu());

        addParam(Knob::small(slug, module, x, levelY, Controls::LevelKnobs + step));

        addParam(Toggle::stepper(2, slug, module, x, shapeY, Controls::ShapeSwitches + step));
        addParam(Knob::small(slug, module, x, curveY, Controls::CurveKnobs + step));

        addParam(Knob::small(slug, module, x, durationY, Controls::DurationKnobs + step));

        addParam(Toggle::button(slug, module, x, enabledButtonY, Controls::EnabledButtons + step));
        addInput(Jack::input(slug, module, x, enabledPortY, Controls::EnabledInputs + step));
      }

      auto constexpr outY = bottom - portRadius - 1.F;
      auto constexpr eosY = top + hp2mm(2.75);

      addInput(Jack::input(slug, module, right, eosY, Controls::CurveSequencerInput));

      addParam(Toggle::stepper(2, slug, module, right, levelY, Controls::LevelRangeSwitch));
      addParam(Toggle::stepper(3, slug, module, right, durationY, Controls::DurationRangeSwitch));
      addOutput(Jack::output(slug, module, right, outY, Controls::CurveSequencerOutput));

      for (auto *popupMenu : popupMenus) {
        addChild(popupMenu);
      }
    }
  }; // namespace dhe
} // namespace curve_sequencer
} // namespace dhe
