#pragma once

#include "mode-ids.h"

#include "controls/switches.h"

namespace dhe {
namespace envelope {
namespace hostage {
struct ModeSwitch {
  static auto constexpr size = mode::labels.size();

  template <typename Panel> using Widget = ThumbSwitch::Widget<Panel, size>;
  using Quantity = rack::engine::SwitchQuantity;

  template <typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    auto *w = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(w);
    return w;
  }

  static inline auto config(rack::engine::Module *module, int param_id,
                            std::string const &name) -> Quantity * {
    auto const labels =
        std::vector<std::string>(mode::labels.cbegin(), mode::labels.cend());
    auto const max_value = static_cast<float>(size - 1);
    auto const default_value = static_cast<float>(ModeId::Hold);
    return module->configSwitch(param_id, 0.F, max_value, default_value, name,
                                labels);
  }
};
} // namespace hostage
} // namespace envelope
} // namespace dhe