#pragma once

#include "./control-ids.h"
#include "widgets/control-widgets.h"
#include "widgets/dimensions.h"
#include <functional>
#include <string>
#include <vector>
#include <widget/SvgWidget.hpp>

namespace dhe {

namespace sequencizer {
template <typename PanelT>
class SelectionKnob : public KnobWidget<PanelT, SmallKnob> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index, std::function<void(int)> const &action)
      -> SelectionKnob * {
    auto knob = rack::createParamCentered<SelectionKnob>(mm2px(xmm, ymm),
                                                         module, index);
    knob->knob_changed_to_ = action;
    return knob;
  }

  void onChange(const rack::event::Change &e) override {
    KnobWidget<PanelT, SmallKnob>::onChange(e);
    auto const value = this->getParamQuantity()->getValue();
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

struct AnchorModeStepper {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("anchor-mode", anchor_mode_count);
    return frame_names;
  }
};

struct AnchorSources {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("anchor-source", anchor_source_count);
    return frame_names;
  }

  template <typename M>
  static inline void config_module(M *module, std::string const &name,
                                   int index) {
    static auto const labels =
        std::vector<std::string>{"No sustain", "Sustain until triggered"};
    module->configParam(module, index, name, labels, 0);
  }

  template <typename P, typename M>
  static inline void config_panel(P *panel, M *module, float xmm, float ymm,
                                  int index) {
    panel->addParam(
        Switches<P>::template create<AnchorSources>(module, xmm, ymm, index));
  }
};

struct AnchorSourceStepper {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("anchor-source", anchor_source_count);
    return frame_names;
  }
};

struct InterruptModeStepper {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("interrupt-mode", interrupt_mode_count);
    return frame_names;
  }
};

struct ShapeStepper {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names = stepper_frame_names("shape", 2);
    return frame_names;
  }
};

struct TriggerModeStepper {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("trigger-mode", trigger_mode_count);
    return frame_names;
  }
};

struct SustainModeStepper {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("sustain-mode", sustain_mode_count);
    return frame_names;
  }
};

} // namespace sequencizer
} // namespace dhe
