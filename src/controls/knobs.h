#pragma once

#include "components/cxmath.h"
#include "components/range.h"
#include "knob-styles.h"
#include "panels/dimensions.h"
#include "panels/panel-assets.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {

template <typename TPanel, typename TStyle, typename TValue = float>
struct KnobWidget : public rack::app::SvgKnob {
  using TAction = std::function<void(TValue)>;

  KnobWidget() {
    auto knob_svg = load_svg(TPanel::svg_dir, TStyle::svg_file);
    setSvg(knob_svg);
    minAngle = -0.83F * pi;
    maxAngle = 0.83F * M_PI;
    shadow->opacity = 0.F;
  }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgKnob::onChange(e);
    auto const value =
        static_cast<TValue>(this->getParamQuantity()->getValue());
    action_(value);
  }

  void on_change(TAction const &action) { action_ = action; }

private:
  TAction action_ = [](TValue) {};
};

template <typename TValue = float>
struct KnobQuantity : public rack::engine::ParamQuantity {
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

struct Knob {
  template <typename TStyle, typename TValue = float, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle, TValue> * {
    auto *widget =
        rack::createParamCentered<KnobWidget<TPanel, TStyle, TValue>>(
            mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  template <typename TQuantity, typename TValue>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, TValue value)
      -> KnobQuantity<TValue> * {
    static auto const range = TQuantity::range();
    return module->configParam<KnobQuantity<TValue>>(
        id, range.lower_bound(), range.upper_bound(), value, name,
        TQuantity::unit, 0.F, TQuantity::display_multiplier,
        TQuantity::display_offset);
  }
};

} // namespace dhe
