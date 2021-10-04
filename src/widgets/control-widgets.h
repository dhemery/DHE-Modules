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

template <typename PanelT> struct Switch {
  template <typename SwitchT> struct Widget : public rack::app::SvgSwitch {
    Widget() {
      auto const panel_prefix = PanelT::svg_dir + "/";
      for (auto const &frame_name : SwitchT::frame_names()) {
        addFrame(load_svg(panel_prefix + frame_name + ".svg"));
      }
      shadow->opacity = 0.F;
    }
  };

  template <typename SwitchT>
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Widget<SwitchT> * {
    return rack::createParamCentered<Widget<SwitchT>>(mm2px(xmm, ymm), module,
                                                      index);
  }

  template <int N> struct Thumb {
    static inline auto frame_names() -> std::vector<std::string> {
      auto names = std::vector<std::string>{};
      auto const prefix = "toggle-" + std::to_string(N) + "-";
      for (size_t position = 1; position <= N; position++) {
        names.push_back(prefix + std::to_string(position));
      }
      return names;
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
      static auto const names = std::vector<std::string>{
          "output-button-released", "output-button-pressed"};
      return names;
    }
  };

  template <typename StepperT>
  static inline auto stepper(rack::engine::Module *module, float xmm, float ymm,
                             int index) -> Widget<StepperT> * {
    return Widget<StepperT>::create(mm2px(xmm, ymm), module, index);
  }

  template <int N>
  static inline auto thumb(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Widget<Thumb<N>> * {
    return create<Thumb<N>>(module, xmm, ymm, index);
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

template <typename PanelT> struct Knob {
  template <typename KnobT> struct Widget : public rack::app::SvgKnob {
    Widget() {
      auto knob_svg = load_svg(PanelT::svg_dir, KnobT::svg_file);
      setSvg(knob_svg);
      minAngle = -0.83F * pi;
      maxAngle = 0.83F * M_PI;
      shadow->opacity = 0.F;
    }
  };

  template <typename KnobT>
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Widget<KnobT> * {
    return rack::createParamCentered<Widget<KnobT>>(mm2px(xmm, ymm), module,
                                                    index);
  }

  struct Large {
    static auto constexpr svg_file = "knob-large";
  };

  struct Medium {
    static auto constexpr svg_file = "knob-medium";
  };

  struct Small {
    static auto constexpr svg_file = "knob-small";
  };

  struct Tiny {
    static auto constexpr svg_file = "knob-tiny";
  };

  static inline auto large(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Widget<Large> * {
    return create<Large>(mm2px(xmm, ymm), module, index);
  }

  static inline auto medium(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Knob * {
    return create<Medium>(mm2px(xmm, ymm), module, index);
  }

  static inline auto small(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Knob * {
    return create<Small>(mm2px(xmm, ymm), module, index);
  }

  static inline auto tiny(rack::engine::Module *module, float xmm, float ymm,
                          int index) -> Knob * {
    return create<Tiny>(mm2px(xmm, ymm), module, index);
  }
};

template <typename PanelT> struct Jack : public rack::app::SvgPort {
  struct Widget : rack::app::SvgPort {
    Widget() {
      setSvg(load_svg(PanelT::svg_dir, "port"));
      shadow->opacity = 0.F;
    }
  };

  static inline auto input(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Widget * {
    return rack::createInputCentered<Widget>(mm2px(xmm, ymm), module, index);
  }

  static inline auto output(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Jack<PanelT>::Widget * {
    return rack::createOutputCentered<Widget>(mm2px(xmm, ymm), module, index);
  }
};
} // namespace dhe
