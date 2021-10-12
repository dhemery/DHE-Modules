#pragma once

#include "./switches.h"
#include "widgets/dimensions.h"

#include <rack.hpp>

#include <string>
#include <vector>

namespace dhe {
template <typename StepperT> struct Stepper {
  using ActionT = std::function<void(typename StepperT::ValueT)>;
  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const frame_names =
        stepper_frame_names(StepperT::frame_prefix, StepperT::labels().size());
    return frame_names;
  }

  template <typename ModuleT>
  static inline void config(ModuleT *module, int id, std::string const &name,
                            typename StepperT::ValueT value) {
    auto const labels = StepperT::labels();
    auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(value);
    module->configSwitch(id, 0.F, max_value, default_value, name, labels);
  }

  template <typename PanelT>
  static inline auto install(
      PanelT *panel, int id, float xmm, float ymm,
      ActionT const &action = [](typename StepperT::ValueT /*selection*/) {})
      -> SwitchWidget<PanelT, Stepper, typename StepperT::ValueT> * {
    auto *w = rack::createParamCentered<
        SwitchWidget<PanelT, Stepper, typename StepperT::ValueT>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    w->set_action(action);
    panel->addParam(w);
    return w;
  }
};
} // namespace dhe
