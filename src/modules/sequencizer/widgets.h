#pragma once

#include "./control-ids.h"
#include "widgets/control-widgets.h"
#include "widgets/dimensions.h"
#include <functional>
#include <widget/SvgWidget.hpp>

namespace dhe {

namespace sequencizer {
template <typename PanelT> class SelectionKnob : public Knob<PanelT>::Small {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index, std::function<void(int)> const &action)
      -> SelectionKnob<PanelT> * {
    auto knob = rack::createParamCentered<SelectionKnob<PanelT>>(
        mm2px(xmm, ymm), module, index);
    knob->knob_changed_to_ = action;
    return knob;
  }

  void onChange(const rack::event::Change &e) override {
    Knob<PanelT>::onChange(e);
    auto const value = this->getParamQuantity()->getDisplayValue();
    auto const selection = static_cast<int>(value);
    knob_changed_to_(selection);
  }

private:
  std::function<void(int)> knob_changed_to_;
};

template <typename PanelT> class StartMarker : public rack::widget::SvgWidget {
public:
  static inline auto create(float step_width, float step_block_x, float ymm)
      -> StartMarker * {
    auto const xmm =
        step_block_x + step_width + step_width / 2.F - light_diameter * 2.F;
    auto marker = rack::createWidgetCentered<StartMarker>(mm2px(xmm, ymm));
    marker->step_block_x_ = step_block_x;
    marker->step_width_ = step_width;
    return marker;
  }

  StartMarker() { setSvg(load_svg(PanelT::svg_dir, "marker-start")); }

  void set_selection_start(int step) {
    auto const x = step_block_x_ + step_width_ * static_cast<float>(step) +
                   step_width_ / 2.F - light_diameter * 2.F;
    this->box.pos.x = mm2px(x);
  }

private:
  float step_block_x_;
  float step_width_;
};

template <template <int> class PanelT, int N>
class EndMarker : public rack::widget::SvgWidget {
public:
  static inline auto create(float step_width, float step_block_x, float ymm)
      -> EndMarker * {
    auto const xmm =
        step_block_x + step_width + step_width / 2.F - light_diameter * 2.F;
    auto marker = rack::createWidgetCentered<EndMarker>(mm2px(xmm, ymm));
    marker->step_block_x_ = step_block_x;
    marker->step_width_ = step_width;
    return marker;
  }

  EndMarker() { setSvg(load_svg(PanelT<N>::svg_dir, "marker-end")); }

  void set_selection_start(int step) {
    this->selection_start_ = step;
    move();
  }

  void set_selection_length(int length) {
    this->selection_length_ = length;
    move();
  }

private:
  void move() {
    auto const selection_end =
        (selection_start_ + selection_length_ - 1) & step_mask_;
    auto const x = step_block_x_ +
                   step_width_ * static_cast<float>(selection_end) +
                   step_width_ / 2.F;
    this->box.pos.x = mm2px(x);
  }

  int selection_start_{};
  int selection_length_{};
  int const step_mask_ = N - 1;
  float step_block_x_;
  float step_width_;
};

template <typename PanelT>
class AnchorModeStepper : public Toggle<PanelT, anchor_mode_count> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> AnchorModeStepper * {
    return rack::createParamCentered<AnchorModeStepper>(mm2px(xmm, ymm), module,
                                                        index);
  }

  AnchorModeStepper() : Toggle<PanelT, anchor_mode_count>{"anchor-mode"} {}
};

template <typename PanelT>
class AnchorSourceStepper : public Toggle<PanelT, anchor_source_count> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> AnchorSourceStepper * {
    return rack::createParamCentered<AnchorSourceStepper>(mm2px(xmm, ymm),
                                                          module, index);
  }

  AnchorSourceStepper()
      : Toggle<PanelT, anchor_source_count>{"anchor-source"} {}
};

template <typename PanelT>
class InterruptModeStepper : public Toggle<PanelT, interrupt_mode_count> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> InterruptModeStepper * {
    return rack::createParamCentered<InterruptModeStepper>(mm2px(xmm, ymm),
                                                           module, index);
  }

  InterruptModeStepper()
      : Toggle<PanelT, interrupt_mode_count>{"interrupt-mode"} {}
};

template <typename PanelT> class ShapeStepper : public Toggle<PanelT, 2> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> ShapeStepper * {
    return rack::createParamCentered<ShapeStepper>(mm2px(xmm, ymm), module,
                                                   index);
  }

  ShapeStepper() : Toggle<PanelT, 2>{"shape"} {}
};

template <typename PanelT>
class TriggerModeStepper : public Toggle<PanelT, trigger_mode_count> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> TriggerModeStepper * {
    return rack::createParamCentered<TriggerModeStepper>(mm2px(xmm, ymm),
                                                         module, index);
  }

  TriggerModeStepper() : Toggle<PanelT, trigger_mode_count>{"trigger-mode"} {}
};

template <typename PanelT>
class SustainModeStepper : public Toggle<PanelT, sustain_mode_count> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> SustainModeStepper * {
    return rack::createParamCentered<SustainModeStepper>(mm2px(xmm, ymm),
                                                         module, index);
  }

  SustainModeStepper() : Toggle<PanelT, sustain_mode_count>{"sustain-mode"} {}
};

} // namespace sequencizer
} // namespace dhe
