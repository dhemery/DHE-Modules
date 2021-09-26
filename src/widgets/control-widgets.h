#pragma once

#include "dimensions.h"
#include "panel-assets.h"

#include <app/SvgSwitch.hpp>
#include <componentlibrary.hpp>
#include <helpers.hpp>
#include <string>

namespace dhe {

class Toggle : public rack::app::SvgSwitch {
public:
  static inline auto thumb(size_t size, std::string const &module_svg_dir,
                           rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Toggle * {
    return new Toggle{size, module_svg_dir, module, xmm, ymm, index};
  }

  //  static inline auto button(std::string const &module_svg_dir,
  //                            rack::engine::Module *module, float xmm, float
  //                            ymm, int index) -> Toggle * {
  //    return new Toggle{"button", 2, module_svg_dir, module, xmm, ymm, index};
  //  }

  static inline auto stepper(std::string const &module_svg_dir,
                             std::string const &name, size_t size,
                             rack::engine::Module *module, float xmm, float ymm,
                             int index) -> Toggle * {
    return new Toggle{name, size, module_svg_dir, module, xmm, ymm, index};
  }

protected:
  Toggle(size_t size, std::string const &module_svg_dir,
         rack::engine::Module *module, float xmm, float ymm, int index)
      : Toggle{"toggle-" + std::to_string(size),
               size,
               module_svg_dir,
               module,
               xmm,
               ymm,
               index} {}

  Toggle(std::string const &toggle_name, size_t size,
         std::string const &module_svg_dir, rack::engine::Module *module,
         float xmm, float ymm, int index) {
    auto const toggle_name_prefix = toggle_name + "-";
    for (size_t position = 1; position <= size; position++) {
      addFrame(load_svg(module_svg_dir,
                        toggle_name_prefix + std::to_string(position)));
    }
    shadow->opacity = 0.F;
    position_centered(this, xmm, ymm);
    momentary = false;
    if (module != nullptr) {
      // TODO: Fix
      // paramQuantity = module->paramQuantities[index];
    }
  }
};

class Button : public rack::app::SvgSwitch {
public:
  static inline auto toggle(std::string const &module_svg_dir,
                            rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Button * {
    return new Button{"button", false, module_svg_dir, module, xmm, ymm, index};
  }

  static inline auto momentary(std::string const &module_svg_dir,
                               rack::engine::Module *module, float xmm,
                               float ymm, int index) -> Button * {
    return new Button{"button", true, module_svg_dir, module, xmm, ymm, index};
  }

  static inline auto output(std::string const &module_svg_dir,
                            rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Button * {
    return new Button("output-button", true, module_svg_dir, module, xmm, ymm,
                      index);
  }

private:
  Button(std::string const &button_name, bool momentary,
         std::string const &module_svg_dir, rack::engine::Module *module,
         float xmm, float ymm, int index) {
    addFrame((load_svg(module_svg_dir, button_name + "-released")));
    addFrame((load_svg(module_svg_dir, button_name + "-pressed")));
    shadow->opacity = 0.F;
    position_centered(this, xmm, ymm);
    rack::app::SvgSwitch::momentary = momentary;
    if (module != nullptr) {
      // TODO: fix
      // paramQuantity = module->paramQuantities[index];
    }
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
    setSvg(load_svg(PanelT::svg_dir, knob_name));
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
                           int index) -> Jack<PanelT>::Widget * {
    return rack::createInputCentered<Jack<PanelT>::Widget>(mm2px(xmm, ymm),
                                                           module, index);
  }

  static inline auto output(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Jack<PanelT>::Widget * {
    return rack::createOutputCentered<Jack<PanelT>::Widget>(mm2px(xmm, ymm),
                                                            module, index);
  }
};
} // namespace dhe
