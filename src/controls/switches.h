#pragma once

#include "panels/dimensions.h"
#include "panels/panel-assets.h"

#include "rack.hpp"

#include <functional>
#include <string>
#include <vector>

namespace dhe {

static inline auto numbered_frame_names(std::string const &frame_prefix,
                                        size_t n) -> std::vector<std::string> {
  auto frame_names = std::vector<std::string>{};
  auto const prefix = frame_prefix + "-";
  for (size_t position = 1; position <= n; position++) {
    frame_names.push_back(prefix + std::to_string(position));
  }
  return frame_names;
}

template <typename TPanel, typename TStyle, typename TValue = int>
struct SwitchWidget : public rack::app::SvgSwitch {
  using TAction = std::function<void(TValue)>;

  SwitchWidget() {
    auto const panel_prefix = std::string{TPanel::svg_dir} + "/";
    for (auto const &frame_name : TStyle::frame_names()) {
      addFrame(load_svg(panel_prefix + frame_name));
    }
    shadow->opacity = 0.F;
  }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgSwitch::onChange(e);
    auto const value =
        static_cast<TValue>(this->getParamQuantity()->getValue());
    action_(value);
  }

  void on_change(TAction const &action) { action_ = action; }

private:
  TAction action_ = [](TValue) {};
};

template <typename TValue = int>
struct SwitchQuantity : public rack::engine::SwitchQuantity {
  using TAction = std::function<void(TValue)>;

  void setValue(float value) override {
    rack::engine::ParamQuantity::setValue(value);
    auto const v = static_cast<TValue>(value);
    action_(v);
  }

  void on_change(TAction const &action) { action_ = action; }

private:
  TAction action_ = [](TValue) {};
};

struct Switch {
  template <typename TStyle, typename TValue = int, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> SwitchWidget<TPanel, TStyle, TValue> * {
    auto *widget =
        rack::createParamCentered<SwitchWidget<TPanel, TStyle, TValue>>(
            mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  template <typename TValue>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            std::vector<std::string> const &labels,
                            TValue initial) -> SwitchQuantity<TValue> * {
    auto const max = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(initial);
    return module->configSwitch<SwitchQuantity<TValue>>(
        id, 0.F, max, default_value, name, labels);
  }
};

template <int N> struct ThumbSwitch {
  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const frame_names =
        numbered_frame_names("toggle-" + std::to_string(N), N);
    return frame_names;
  }

  template <typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> SwitchWidget<TPanel, ThumbSwitch<N>> * {
    return Switch::install<ThumbSwitch<N>>(panel, id, xmm, ymm);
  }
};

template <typename TStepper> struct Stepper {
  using TValue = typename TStepper::TValue;
  using TQuantity = SwitchQuantity<TValue>;
  using TFrame = Stepper<TStepper>;
  template <typename TPanel>
  using TWidget = SwitchWidget<TPanel, TFrame, TValue>;

  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const frame_names =
        numbered_frame_names(TStepper::frame_prefix, TStepper::labels().size());
    return frame_names;
  }

  template <typename TModule>
  static inline auto config(TModule *module, int id, std::string const &name,
                            TValue value) -> TQuantity * {
    return Switch::config(module, id, name, TStepper::labels(), value);
  }

  template <typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> TWidget<TPanel> * {
    return Switch::install<TFrame, TValue>(panel, id, xmm, ymm);
  }
};

struct Normal {
  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const names =
        std::vector<std::string>{"button-released", "button-pressed"};
    return names;
  }
};

struct Reversed {
  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const names = std::vector<std::string>{"output-button-released",
                                                       "output-button-pressed"};
    return names;
  }
};

struct Momentary {
  static auto constexpr momentary = true;
};

struct Toggle {
  static auto constexpr momentary = false;
};

struct Button {
  template <typename TBehavior, typename TStyle = Normal, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> SwitchWidget<TPanel, TStyle, bool> * {
    auto widget = rack::createParamCentered<SwitchWidget<TPanel, TStyle, bool>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    widget->momentary = TBehavior::momentary;
    panel->addParam(widget);
    return widget;
  }

  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, bool pressed = false)
      -> SwitchQuantity<bool> * {
    auto const default_value = static_cast<float>(pressed);
    auto *pq = module->configSwitch<SwitchQuantity<bool>>(id, 0.F, 1.F,
                                                          default_value, name);
    return pq;
  }
};
} // namespace dhe
