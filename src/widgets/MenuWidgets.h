#pragma once
#include <app/ParamWidget.hpp>
#include <engine/Module.hpp>

namespace dhe {
class Menu : public rack::app::ParamWidget {
public:
  static inline auto button(rack::engine::Module *module, float x, float y, int index) -> Menu * {
    return new Menu(module, x, y, index);
  }

private:
  Menu(rack::engine::Module *module, float x, float y, int index) {
    positionCentered(this, x, y);
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }
  }
};

} // namespace dhe
