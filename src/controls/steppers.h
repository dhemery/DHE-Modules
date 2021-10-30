#pragma once

#include "switches.h"

#include "panels/dimensions.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {
template <typename TStepper> struct Stepper {
  using TValue = typename TStepper::TValue;
  using TAction = std::function<void(TValue)>;
  template <typename TPanel>
  using TWidget = Switch::Widget<TPanel, Stepper, TValue>;

  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const frame_names =
        numbered_frame_names(TStepper::frame_prefix, TStepper::labels().size());
    return frame_names;
  }

  template <typename ModuleT>
  static inline void config(ModuleT *module, int id, std::string const &name,
                            TValue value) {
    auto const labels = TStepper::labels();
    auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(value);
    module->configSwitch(id, 0.F, max_value, default_value, name, labels);
  }

  template <typename PanelT>
  static inline auto install(
      PanelT *panel, int id, float xmm, float ymm,
      TAction const &action = [](TValue) {}) -> TWidget<PanelT> * {
    auto *w = rack::createParamCentered<TWidget<PanelT>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    w->set_action(action);
    panel->addParam(w);
    return w;
  }
};
} // namespace dhe
