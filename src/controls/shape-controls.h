#pragma once

#include "app/SvgSwitch.hpp"
#include "engine/ParamQuantity.hpp"
#include "signals/shape-signals.h"
#include "switches.h"

#include "rack.hpp"

#include <string>

namespace dhe {
struct CurvatureKnob {
  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return Curvature::scale(getValue());
    }

    void setDisplayValue(float display_value) override {
      setValue(Curvature::normalize(display_value));
    }
  };

  static auto config(rack::engine::Module *module, int id,
                     std::string const &name, float default_curvature = 0.F)
      -> Quantity * {
    auto const default_rotation = Curvature::normalize(default_curvature);
    auto *q =
        module->configParam<Quantity>(id, 0.F, 1.F, default_rotation, name, " Hz");
    return q;
  }
};

struct ShapeSwitch {
  using Quantity = rack::engine::SwitchQuantity;
  template <typename Panel>
  using Widget = ThumbSwitch::Widget<Panel, shape::labels.size()>;

  template <typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    auto *w = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(w);
    return w;
  }

  static inline auto config(rack::engine::Module *module, int param_id,
                            std::string const &name,
                            Shape::Id default_shape = Shape::Id::J)
      -> Quantity * {
    static auto const labels =
        std::vector<std::string>{shape::labels.cbegin(), shape::labels.cend()};
    static auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(default_shape);
    auto *q = module->configSwitch<Quantity>(param_id, 0.F, max_value,
                                             default_value, name, labels);
    return q;
  }
};

struct ShapeStepper {
  template <typename Panel> struct Widget : rack::app::SvgSwitch {
    static auto constexpr size = shape::labels.size();
    Widget() {
      auto const prefix = std::string{Panel::svg_dir} + "/shape-";
      for (size_t position = 1; position <= size; position++) {
        addFrame(load_svg(prefix + std::to_string(position)));
      }
      shadow->opacity = 0.F;
    }
  };

  template <typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    auto *w = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(w);
    return w;
  }
};
} // namespace dhe