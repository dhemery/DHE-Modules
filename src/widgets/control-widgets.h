#pragma once

#include "dimensions.h"
#include "panel-assets.h"

#include <app/SvgKnob.hpp>
#include <app/SvgPort.hpp>
#include <app/SvgSwitch.hpp>
#include <componentlibrary.hpp>
#include <helpers.hpp>
#include <string>

namespace dhe {

template <typename PanelT, int N> class Toggle : public rack::app::SvgSwitch {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Toggle * {
    return rack::createParamCentered<Toggle>(mm2px(xmm, ymm), module, index);
  }

  Toggle() : Toggle{"toggle-" + std::to_string(N)} {}

protected:
  Toggle(std::string const &name) {
    auto const toggle_name_prefix = name + "-";
    for (size_t position = 1; position <= N; position++) {
      addFrame(load_svg(PanelT::svg_dir,
                        toggle_name_prefix + std::to_string(position)));
    }
    shadow->opacity = 0.F;
  }
};

template <typename PanelT> class Button : public rack::app::SvgSwitch {
public:
  static inline auto toggle(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Button * {
    return rack::createParamCentered<Button>(mm2px(xmm, ymm), module, index);
  }

  static inline auto momentary(rack::engine::Module *module, float xmm,
                               float ymm, int index) -> Button * {
    auto button =
        rack::createParamCentered<Button>(mm2px(xmm, ymm), module, index);
    button->rack::app::SvgSwitch::momentary = true;
    return button;
  }

  static inline auto output(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Button * {
    auto button =
        rack::createParamCentered<OutputButton>(mm2px(xmm, ymm), module, index);
    button->rack::app::SvgSwitch::momentary = true;
    return button;
  }

  Button() : Button{"button"} {}

  class OutputButton : public Button {
  public:
    OutputButton() : Button{"output-button"} {}
  };

private:
  Button(std::string const &button_name) {
    addFrame(load_svg(PanelT::svg_dir, button_name + "-released"));
    addFrame(load_svg(PanelT::svg_dir, button_name + "-pressed"));
    shadow->opacity = 0.F;
  }
};

template <typename PanelT>
class Knob : public rack::componentlibrary::RoundKnob {
public:
  static inline auto large(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Knob * {
    return rack::createParamCentered<Large>(mm2px(xmm, ymm), module, index);
  }

  static inline auto medium(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Knob * {
    return rack::createParamCentered<Medium>(mm2px(xmm, ymm), module, index);
  }

  static inline auto small(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Knob * {
    return rack::createParamCentered<Small>(mm2px(xmm, ymm), module, index);
  }

  static inline auto tiny(rack::engine::Module *module, float xmm, float ymm,
                          int index) -> Knob * {
    return rack::createParamCentered<Tiny>(mm2px(xmm, ymm), module, index);
  }

  Knob(std::string const &knob_name) {
    auto knob_svg = load_svg(PanelT::svg_dir, knob_name);
    setSvg(knob_svg);
    bg->setSvg(knob_svg);
    shadow->opacity = 0.F;
  }

  class Large : public Knob {
  public:
    Large() : Knob{"knob-large"} {}
  };

  class Medium : public Knob {
  public:
    Medium() : Knob{"knob-medium"} {}
  };

  class Small : public Knob {
  public:
    Small() : Knob{"knob-small"} {}
  };

  class Tiny : public Knob {
  public:
    Tiny() : Knob{"knob-tiny"} {}
  };
};

template <typename PanelT> class Jack : public rack::app::SvgPort {
  struct Widget : rack::app::SvgPort {
    Widget() {
      setSvg(load_svg(PanelT::svg_dir, "port"));
      shadow->opacity = 0.F;
    }
  };

public:
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
