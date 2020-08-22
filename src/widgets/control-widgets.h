#pragma once

#include "dimensions.h"
#include "panel-assets.h"

#include <app/SvgSwitch.hpp>
#include <componentlibrary.hpp>
#include <string>

namespace dhe {

class Toggle : public rack::app::SvgSwitch {
public:
  static inline auto stepper(size_t size, std::string const &module_svg_dir,
                             rack::engine::Module *module, float xmm, float ymm,
                             int index) -> Toggle * {
    return new Toggle{size, module_svg_dir, module, xmm, ymm, index};
  }

  static inline auto button(std::string const &module_svg_dir,
                            rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Toggle * {
    return new Toggle{"button", 2, module_svg_dir, module, xmm, ymm, index};
  }

  static inline auto buttons(std::string const &module_svg_dir,
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
      addFrame(control_svg(module_svg_dir,
                           toggle_name_prefix + std::to_string(position)));
    }
    shadow->opacity = 0.F;
    position_centered(this, xmm, ymm);
    momentary = false;
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }
  }
};

class Button : public Toggle {
public:
  static inline auto momentary(std::string const &module_svg_dir,
                               rack::engine::Module *module, float xmm,
                               float ymm, int index) -> Button * {
    return new Button{"button", module_svg_dir, module, xmm, ymm, index};
  }

  static inline auto reverse(std::string const &module_svg_dir,
                             rack::engine::Module *module, float xmm, float ymm,
                             int index) -> Button * {
    return new Button("button-reversed", module_svg_dir, module, xmm, ymm,
                      index);
  }

private:
  Button(std::string const &button_name, std::string const &module_svg_dir,
         rack::engine::Module *module, float xmm, float ymm, int index)
      : Toggle{button_name, 2, module_svg_dir, module, xmm, ymm, index} {
    Toggle::momentary = true;
  }
};

class Knob : public rack::componentlibrary::RoundKnob {
public:
  static inline auto large(std::string const &module_svg_dir,
                           rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Knob * {
    return new Knob{module_svg_dir, "knob-large", module, xmm, ymm, index};
  }

  static inline auto medium(std::string const &module_svg_dir,
                            rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Knob * {
    return new Knob{module_svg_dir, "knob-medium", module, xmm, ymm, index};
  }

  static inline auto small(std::string const &module_svg_dir,
                           rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Knob * {
    return new Knob{module_svg_dir, "knob-small", module, xmm, ymm, index};
  }

  static inline auto tiny(std::string const &module_svg_dir,
                          rack::engine::Module *module, float xmm, float ymm,
                          int index) -> Knob * {
    return new Knob{module_svg_dir, "knob-tiny", module, xmm, ymm, index};
  }

protected:
  Knob(std::string const &module_svg_dir, std::string const &knob_name,
       rack::engine::Module *module, float xmm, float ymm, int index) {
    setSvg(control_svg(module_svg_dir, knob_name));
    shadow->opacity = 0.F;
    position_centered(this, xmm, ymm);
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }
  }
};

class Jack : public rack::app::SvgPort {
public:
  static inline auto input(std::string const &module_svg_dir,
                           rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Jack * {
    return new Jack{rack::app::PortWidget::Type::INPUT,
                    module_svg_dir,
                    module,
                    xmm,
                    ymm,
                    index};
  }

  static inline auto output(std::string const &module_svg_dir,
                            rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Jack * {
    return new Jack{rack::app::PortWidget::Type::OUTPUT,
                    module_svg_dir,
                    module,
                    xmm,
                    ymm,
                    index};
  }

private:
  Jack(rack::app::PortWidget::Type type, std::string const &module_svg_dir,
       rack::engine::Module *module, float xmm, float ymm, int index) {
    setSvg(control_svg(module_svg_dir, "port"));
    shadow->opacity = 0.F;
    position_centered(this, xmm, ymm);
    this->module = module;
    this->type = type;
    portId = index;
  }
};
} // namespace dhe
