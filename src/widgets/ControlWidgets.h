#pragma once

#include "Dimensions.h"
#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <componentlibrary.hpp>
#include <string>

namespace dhe {

class Toggle : public rack::app::SvgSwitch {
public:
  static inline auto stepper(int size, std::string const &moduleSvgDir, rack::engine::Module *module, float xmm,
                             float ymm, int index) -> Toggle * {
    return new Toggle{size, moduleSvgDir, module, xmm, ymm, index};
  }

  static inline auto button(std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Toggle * {
    return new Toggle{"button", 2, moduleSvgDir, module, xmm, ymm, index};
  }

protected:
  Toggle(int size, std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm, int index) :
      Toggle{"toggle-" + std::to_string(size), size, moduleSvgDir, module, xmm, ymm, index} {}

  Toggle(std::string const &toggleName, int size, std::string const &moduleSvgDir, rack::engine::Module *module,
         float xmm, float ymm, int index) {
    auto const toggleNamePrefix = toggleName + "-";
    for (auto position = 1; position <= size; position++) {
      addFrame(controlSvg(moduleSvgDir, toggleNamePrefix + std::to_string(position)));
    }
    shadow->opacity = 0.F;
    positionCentered(this, xmm, ymm);
    momentary = false;
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }
  }
};

class Button : public Toggle {
public:
  static inline auto momentary(std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                               int index) -> Button * {
    return new Button{"button", moduleSvgDir, module, xmm, ymm, index};
  }

  static inline auto reverse(std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                             int index) -> Button * {
    return new Button("button-reversed", moduleSvgDir, module, xmm, ymm, index);
  }

private:
  Button(std::string const &buttonName, std::string const &moduleSvgDir, rack::engine::Module *module, float xmm,
         float ymm, int index) :
      Toggle{buttonName, 2, moduleSvgDir, module, xmm, ymm, index} {
    Toggle::momentary = true;
  }
};

class Knob : public rack::componentlibrary::RoundKnob {
public:
  static inline auto large(std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Knob * {
    return new Knob{moduleSvgDir, "knob-large", module, xmm, ymm, index};
  }

  static inline auto medium(std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Knob * {
    return new Knob{moduleSvgDir, "knob-medium", module, xmm, ymm, index};
  }

  static inline auto small(std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Knob * {
    return new Knob{moduleSvgDir, "knob-small", module, xmm, ymm, index};
  }

  static inline auto tiny(std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                          int index) -> Knob * {
    return new Knob{moduleSvgDir, "knob-tiny", module, xmm, ymm, index};
  }

protected:
  Knob(std::string const &moduleSvgDir, std::string const &knobName, rack::engine::Module *module, float xmm, float ymm,
       int index) {
    setSvg(controlSvg(moduleSvgDir, knobName));
    shadow->opacity = 0.F;
    positionCentered(this, xmm, ymm);
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }
  }
};

class Jack : public rack::app::SvgPort {
public:
  static inline auto input(std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Jack * {
    return new Jack{rack::app::PortWidget::Type::INPUT, moduleSvgDir, module, xmm, ymm, index};
  }

  static inline auto output(std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Jack * {
    return new Jack{rack::app::PortWidget::Type::OUTPUT, moduleSvgDir, module, xmm, ymm, index};
  }

private:
  Jack(rack::app::PortWidget::Type type, std::string const &moduleSvgDir, rack::engine::Module *module, float xmm,
       float ymm, int index) {
    setSvg(controlSvg(moduleSvgDir, "port"));
    shadow->opacity = 0.F;
    positionCentered(this, xmm, ymm);
    this->module = module;
    this->type = type;
    portId = index;
  }
};
} // namespace dhe
