#pragma once

#include "components/cxmath.h"
#include "dimensions.h"
#include "panel-assets.h"

#include <app/SvgKnob.hpp>
#include <app/SvgPort.hpp>
#include <app/SvgSwitch.hpp>
#include <helpers.hpp>
#include <string>
#include <vector>

namespace dhe {

template <typename PanelT, typename SwitchT>
struct SwitchWidget : public rack::app::SvgSwitch {
  SwitchWidget() {
    auto const panel_prefix = std::string{PanelT::svg_dir} + "/";
    for (auto const &frame_name : SwitchT::frame_names()) {
      addFrame(load_svg(panel_prefix + frame_name));
    }
    shadow->opacity = 0.F;
  }
};

static inline auto stepper_frame_names(std::string const &stepper_name,
                                       size_t n) -> std::vector<std::string> {
  auto frame_names = std::vector<std::string>{};
  auto const prefix = stepper_name + "-";
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
};

struct Button {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const names =
        std::vector<std::string>{"button-released", "button-pressed"};
    return names;
  }
};

struct OutputButton {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const names = std::vector<std::string>{"output-button-released",
                                                       "output-button-pressed"};
    return names;
  }
};

template <typename PanelT> struct Switches {
  template <typename SwitchT> using Widget = SwitchWidget<PanelT, SwitchT>;

  template <typename SwitchT>
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Widget<SwitchT> * {
    return rack::createParamCentered<Widget<SwitchT>>(mm2px(xmm, ymm), module,
                                                      index);
  }

  template <int N>
  static inline auto thumb(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Widget<ThumbSwitch<N>> * {
    return create<ThumbSwitch<N>>(module, xmm, ymm, index);
  }

  template <typename ButtonT = Button>
  static inline auto toggle(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Widget<ButtonT> * {
    return create<ButtonT>(module, xmm, ymm, index);
  }

  template <typename ButtonT = Button>
  static inline auto momentary(rack::engine::Module *module, float xmm,
                               float ymm, int index) -> Widget<ButtonT> * {
    auto button = create<ButtonT>(module, xmm, ymm, index);
    button->momentary = true;
    return button;
  }
};

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

template <typename PanelT> struct JackWidget : rack::app::SvgPort {
  JackWidget() {
    setSvg(load_svg(PanelT::svg_dir, "port"));
    shadow->opacity = 0.F;
  }
};

template <typename PanelT> struct Jacks : public rack::app::SvgPort {

  static inline auto input(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> JackWidget<PanelT> * {
    return rack::createInputCentered<JackWidget<PanelT>>(mm2px(xmm, ymm),
                                                         module, index);
  }

  static inline auto output(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> JackWidget<PanelT> * {
    return rack::createOutputCentered<JackWidget<PanelT>>(mm2px(xmm, ymm),
                                                          module, index);
  }
};
} // namespace dhe
