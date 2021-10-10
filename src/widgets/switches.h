#pragma once

#include "dimensions.h"
#include "panel-assets.h"

#include <app/SvgSwitch.hpp>
#include <helpers.hpp>

#include <string>
#include <vector>

namespace dhe {

template <typename PanelT, typename FrameT, typename ValueT>
struct SwitchWidget : public rack::app::SvgSwitch {
  using ActionT = std::function<void(ValueT)>;

  SwitchWidget() {
    auto const panel_prefix = std::string{PanelT::svg_dir} + "/";
    for (auto const &frame_name : FrameT::frame_names()) {
      addFrame(load_svg(panel_prefix + frame_name));
    }
    shadow->opacity = 0.F;
  }

  void set_action(ActionT const &action) { action_ = action; }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgSwitch::onChange(e);
    auto const value =
        static_cast<ValueT>(this->getParamQuantity()->getValue());
    action_(value);
  }

private:
  ActionT action_ = [](ValueT /*selection*/) {};
};

static inline auto stepper_frame_names(std::string const &frame_prefix,
                                       size_t n) -> std::vector<std::string> {
  auto frame_names = std::vector<std::string>{};
  auto const prefix = frame_prefix + "-";
  for (size_t position = 1; position <= n; position++) {
    frame_names.push_back(prefix + std::to_string(position));
  }
  return frame_names;
}

template <int N> struct ThumbSwitch {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("toggle-" + std::to_string(N), N);
    return frame_names;
  }

  template <typename PanelT>
  static inline void install(PanelT *panel, int id, float xmm, float ymm) {
    panel->addParam(
        rack::createParamCentered<SwitchWidget<PanelT, ThumbSwitch, int>>(
            mm2px(xmm, ymm), panel->getModule(), id));
  }
};

struct Normal {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const names =
        std::vector<std::string>{"button-released", "button-pressed"};
    return names;
  }
};

struct Reversed {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const names = std::vector<std::string>{"output-button-released",
                                                       "output-button-pressed"};
    return names;
  }
};

struct Momentary {
  template <typename WidgetT>
  static inline void init_behavior(WidgetT *widget) {
    widget->momentary = true;
  }
};

struct Toggle {
  template <typename WidgetT>
  static inline void init_behavior(WidgetT *widget) {
    widget->momentary = false;
  }
};

struct Button {
  template <typename BehaviorT, typename FrameT = Normal, typename PanelT>
  static inline void install(PanelT *panel, int id, float xmm, float ymm) {
    using WidgetT = SwitchWidget<PanelT, FrameT, bool>;
    auto w = rack::createParamCentered<WidgetT>(mm2px(xmm, ymm),
                                                panel->getModule(), id);
    BehaviorT::init_behavior(w);
    panel->addParam(w);
  }
};

template <typename StepperT> struct Stepper {
  using ActionT = std::function<void(typename StepperT::ValueT)>;
  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const frame_names =
        stepper_frame_names(StepperT::frame_prefix, StepperT::labels().size());
    return frame_names;
  }

  template <typename ModuleT>
  static inline void config(ModuleT *module, int id, std::string const &name,
                            typename StepperT::ValueT value) {
    auto const labels = StepperT::labels();
    auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(value);
    module->configSwitch(id, 0.F, max_value, default_value, name, labels);
  }

  template <typename PanelT>
  static inline auto install(
      PanelT *panel, int id, float xmm, float ymm,
      ActionT const &action = [](typename StepperT::ValueT /*unused*/) {})
      -> SwitchWidget<PanelT, Stepper, typename StepperT::ValueT> * {
    auto *w = rack::createParamCentered<
        SwitchWidget<PanelT, Stepper, typename StepperT::ValueT>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    w->set_action(action);
    panel->addParam(w);
    return w;
  }
};

template <typename PanelT> struct Switches {
  template <typename FrameT> using Widget = SwitchWidget<PanelT, FrameT, int>;

  template <typename FrameT>
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Widget<FrameT> * {
    return rack::createParamCentered<Widget<FrameT>>(mm2px(xmm, ymm), module,
                                                     index);
  }

  template <typename FrameT = Normal>
  static inline auto toggle(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Widget<FrameT> * {
    return create<FrameT>(module, xmm, ymm, index);
  }

  template <typename FrameT = Normal>
  static inline auto momentary(rack::engine::Module *module, float xmm,
                               float ymm, int index) -> Widget<FrameT> * {
    auto button = create<FrameT>(module, xmm, ymm, index);
    button->momentary = true;
    return button;
  }
};

} // namespace dhe
