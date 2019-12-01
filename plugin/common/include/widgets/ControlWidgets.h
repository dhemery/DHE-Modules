#pragma once

#include "CommonWidgets.h"
#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <componentlibrary.hpp>
#include <string>

namespace dhe {

class Button : public rack::app::SvgSwitch {
public:
  static inline auto reverse(std::string const &moduleSlug, rack::engine::Module *module, float x, float y, int index)
      -> Button * {
    return new Button{moduleSlug, "button-reversed", true, module, x, y, index};
  }

  static inline auto toggle(std::string const &moduleSlug, rack::engine::Module *module, float x, float y, int index)
      -> Button * {
    return new Button{moduleSlug, "button", false, module, x, y, index};
  }

  static inline auto momentary(std::string const &moduleSlug, rack::engine::Module *module, float x, float y, int index)
      -> Button * {
    return new Button{moduleSlug, "button", true, module, x, y, index};
  }

private:
  Button(std::string const &moduleSlug, std::string const &buttonName, bool isMomentary, rack::engine::Module *module,
         float x, float y, int index) {
    rack::app::SvgSwitch::momentary = isMomentary;
    this->addFrame(controlSvg(moduleSlug, buttonName + "-1"));
    this->addFrame(controlSvg(moduleSlug, buttonName + "-2"));
    shadow->opacity = 0.F;
    positionCentered(this, x, y);
    if (module) {
      paramQuantity = module->paramQuantities[index];
    }
  }
};

class Knob : public rack::componentlibrary::RoundKnob {
public:
  static inline auto large(std::string const &moduleSlug, rack::engine::Module *module, float x, float y, int index)
      -> Knob * {
    return new Knob{moduleSlug, "knob-large", module, x, y, index};
  }

  static inline auto medium(std::string const &moduleSlug, rack::engine::Module *module, float x, float y, int index)
      -> Knob * {
    return new Knob{moduleSlug, "knob-medium", module, x, y, index};
  }

  static inline auto small(std::string const &moduleSlug, rack::engine::Module *module, float x, float y, int index)
      -> Knob * {
    return new Knob{moduleSlug, "knob-small", module, x, y, index};
  }

  static inline auto tiny(std::string const &moduleSlug, rack::engine::Module *module, float x, float y, int index)
      -> Knob * {
    return new Knob{moduleSlug, "knob-tiny", module, x, y, index};
  }

protected:
  Knob(std::string const &moduleSlug, std::string const &knobName, rack::engine::Module *module, float x, float y,
       int index) {
    setSvg(controlSvg(moduleSlug, knobName));
    shadow->opacity = 0.F;
    positionCentered(this, x, y);
    if (module) {
      paramQuantity = module->paramQuantities[index];
    }
  }
};

template <int N> class Toggle : public rack::app::SvgSwitch {
public:
  explicit Toggle(std::string name = "toggle-" + std::to_string(N)) : toggleName{std::move(name)} {}

  void setGraphics(std::string const &moduleSlug, float x, float y) {
    auto const controlNamePrefix = toggleName + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(controlSvg(moduleSlug, controlNamePrefix + std::to_string(position)));
    }
    positionCentered(this, x, y);
  }

private:
  std::string const toggleName;
  static constexpr auto size = N;
};

} // namespace dhe
