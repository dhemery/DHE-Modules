#pragma once

#include "app/SvgSwitch.hpp"
#include "engine/ParamQuantity.hpp"
#include "signals/curvature.h"
#include "signals/shape.h"
#include "switches.h"

#include <string>

namespace dhe {
struct CurvatureKnob {
  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return curvature::scale(getValue());
    }

    void setDisplayValue(float display_value) override {
      setValue(curvature::normalize(display_value));
    }
  };

  static auto config(rack::engine::Module *module, int id,
                     std::string const &name, float default_curvature = 0.F)
      -> Quantity * {
    auto const default_rotation = curvature::normalize(default_curvature);
    auto *quantity =
        module->configParam<Quantity>(id, 0.F, 1.F, default_rotation, name);
    return quantity;
  }
};

struct ShapeSwitch {
  using Quantity = rack::engine::SwitchQuantity;
  template <typename Panel>
  using Widget = ThumbSwitch::Widget<Panel, Shape::count>;

  template <typename Panel>
  static auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    auto *param = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(param);
    return param;
  }

  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name, int default_shape = 0)
      -> Quantity * {
    static auto const max_value = static_cast<float>(Shape::count - 1);
    auto const default_value = static_cast<float>(default_shape);
    auto *quantity = module->configSwitch<Quantity>(
        param_id, 0.F, max_value, default_value, name, Shape::labels());
    return quantity;
  }
};

struct ShapeStepper {
  template <typename Panel> struct Widget : rack::app::SvgSwitch {
    static auto constexpr size = Shape::count;
    Widget() {
      auto const prefix = std::string{Panel::svg_dir} + "/shape-";
      for (size_t position = 1; position <= size; position++) {
        addFrame(load_svg(prefix + std::to_string(position)));
      }
      shadow->opacity = 0.F;
    }
  };

  template <typename Panel>
  static auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    auto *widget = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(widget);
    return widget;
  }
};
} // namespace dhe
