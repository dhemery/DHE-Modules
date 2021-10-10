#pragma once

#include "./switches.h"
#include "widgets/dimensions.h"

#include <helpers.hpp>

#include <string>
#include <vector>

namespace dhe {
struct Normal {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const names =
        std::vector<std::string>{"button-released", "button-pressed"};
    return names;
  }
};

struct Reversed {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const names = std::vector<std::string>{"output-button-released",
                                                       "output-button-pressed"};
    return names;
  }
};

struct Momentary {
  template <typename WidgetT>
  static inline void init_behavior(WidgetT *widget) {
    widget->momentary = true;
  }
};

struct Toggle {
  template <typename WidgetT>
  static inline void init_behavior(WidgetT *widget) {
    widget->momentary = false;
  }
};

struct Button {
  template <typename BehaviorT, typename FrameT = Normal, typename PanelT>
  static inline void install(PanelT *panel, int id, float xmm, float ymm) {
    using WidgetT = SwitchWidget<PanelT, FrameT, bool>;
    auto w = rack::createParamCentered<WidgetT>(mm2px(xmm, ymm),
                                                panel->getModule(), id);
    BehaviorT::init_behavior(w);
    panel->addParam(w);
  }
};
} // namespace dhe
