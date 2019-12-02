#pragma once

#include "Dimensions.h"
#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <componentlibrary.hpp>
#include <string>

namespace dhe {

static inline void positionCentered(rack::widget::Widget *widget, float xmm, float ymm) {
  widget->box.pos = mm2px(xmm, ymm).minus(widget->box.size.div(2));
}

class Toggle : public rack::app::SvgSwitch {
public:
  static inline auto stepper(int size, std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y,
                             int index) -> Toggle * {
    return new Toggle{size, moduleSvgDir, module, x, y, index};
  }

  static inline auto button(std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y, int index)
      -> Toggle * {
    return new Toggle{"button", 2, moduleSvgDir, module, x, y, index};
  }

protected:
  Toggle(int size, std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y, int index) :
      Toggle{"toggle-" + std::to_string(size), size, moduleSvgDir, module, x, y, index} {}

  Toggle(std::string const &toggleName, int size, std::string const &moduleSvgDir, rack::engine::Module *module,
         float x, float y, int index) {
    auto const toggleNamePrefix = toggleName + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(controlSvg(moduleSvgDir, toggleNamePrefix + std::to_string(position)));
    }
    shadow->opacity = 0.F;
    positionCentered(this, x, y);
    momentary = false;
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }
  }
};

class Button : public Toggle {
public:
  static inline auto momentary(std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y,
                               int index) -> Button * {
    return new Button{"button", moduleSvgDir, module, x, y, index};
  }

  static inline auto reverse(std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y, int index)
      -> Button * {
    return new Button("button-reversed", moduleSvgDir, module, x, y, index);
  }

private:
  Button(std::string const &buttonName, std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y,
         int index) :
      Toggle{buttonName, 2, moduleSvgDir, module, x, y, index} {
    Toggle::momentary = true;
  }
};

class Knob : public rack::componentlibrary::RoundKnob {
public:
  static inline auto large(std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y, int index)
      -> Knob * {
    return new Knob{moduleSvgDir, "knob-large", module, x, y, index};
  }

  static inline auto medium(std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y, int index)
      -> Knob * {
    return new Knob{moduleSvgDir, "knob-medium", module, x, y, index};
  }

  static inline auto small(std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y, int index)
      -> Knob * {
    return new Knob{moduleSvgDir, "knob-small", module, x, y, index};
  }

  static inline auto tiny(std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y, int index)
      -> Knob * {
    return new Knob{moduleSvgDir, "knob-tiny", module, x, y, index};
  }

protected:
  Knob(std::string const &moduleSvgDir, std::string const &knobName, rack::engine::Module *module, float x, float y,
       int index) {
    setSvg(controlSvg(moduleSvgDir, knobName));
    shadow->opacity = 0.F;
    positionCentered(this, x, y);
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }
  }
};

class Jack : public rack::app::SvgPort {
public:
  static inline auto input(std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y, int index)
      -> Jack * {
    return new Jack{rack::app::PortWidget::Type::INPUT, moduleSvgDir, module, x, y, index};
  }

  static inline auto output(std::string const &moduleSvgDir, rack::engine::Module *module, float x, float y, int index)
      -> Jack * {
    return new Jack{rack::app::PortWidget::Type::OUTPUT, moduleSvgDir, module, x, y, index};
  }

private:
  Jack(rack::app::PortWidget::Type type, std::string const &moduleSvgDir, rack::engine::Module *module, float x,
       float y, int index) {
    setSvg(controlSvg(moduleSvgDir, "port"));
    shadow->opacity = 0.F;
    positionCentered(this, x, y);
    this->module = module;
    this->type = type;
    portId = index;
  }
};
} // namespace dhe
