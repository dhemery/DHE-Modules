#pragma once

#include "components/cxmath.h"
#include "dimensions.h"
#include "panel-assets.h"

#include <app/SvgKnob.hpp>
#include <helpers.hpp>

#include <string>

namespace dhe {
template <typename PanelT, typename KnobT>
struct KnobWidget : public rack::app::SvgKnob {
  KnobWidget() {
    auto knob_svg = load_svg(PanelT::svg_dir, KnobT::svg_file);
    setSvg(knob_svg);
    minAngle = -0.83F * pi;
    maxAngle = 0.83F * M_PI;
    shadow->opacity = 0.F;
  }
};

struct LargeKnob {
  static auto constexpr svg_file = "knob-large";
};

struct MediumKnob {
  static auto constexpr svg_file = "knob-medium";
};

struct SmallKnob {
  static auto constexpr svg_file = "knob-small";
};

struct TinyKnob {
  static auto constexpr svg_file = "knob-tiny";
};

template <typename PanelT> struct Knobs {
  template <typename KnobT> using Widget = KnobWidget<PanelT, KnobT>;

  template <typename KnobT>
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Widget<KnobT> * {
    return rack::createParamCentered<KnobWidget<PanelT, KnobT>>(mm2px(xmm, ymm),
                                                                module, index);
  }

  static inline auto large(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Widget<LargeKnob> * {
    return create<LargeKnob>(module, xmm, ymm, index);
  }

  static inline auto medium(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Widget<MediumKnob> * {
    return create<MediumKnob>(module, xmm, ymm, index);
  }

  static inline auto small(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Widget<SmallKnob> * {
    return create<SmallKnob>(module, xmm, ymm, index);
  }

  static inline auto tiny(rack::engine::Module *module, float xmm, float ymm,
                          int index) -> Widget<TinyKnob> * {
    return create<TinyKnob>(module, xmm, ymm, index);
  }
};

struct Attenuator {
  template <typename SizeT, typename PanelT>
  static inline void install(PanelT *panel, float xmm, float ymm, int id) {
    panel->addParam(rack::createParamCentered<KnobWidget<PanelT, SizeT>>(
        mm2px(xmm, ymm), panel->getModule(), id));
  }

  template <typename ModuleT>
  static inline void config(ModuleT *module, int id, std::string const &name,
                            float rotation = 1.F) {
    module->configParam(id, 0.F, 1.F, rotation, name, "%", 0.F, 100.F, 0.F);
  }
};
} // namespace dhe
