#pragma once

#include "./switches.h"
#include "widgets/dimensions.h"

#include "rack.hpp"

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
  template <typename TWidget>
  static inline void init_behavior(TWidget *widget) {
    widget->momentary = true;
  }
};

struct Toggle {
  template <typename TWidget>
  static inline void init_behavior(TWidget *widget) {
    widget->momentary = false;
  }
};

struct Button {
  template <typename BehaviorT, typename TFrame = Normal, typename TPanel>
  static inline void install(TPanel *panel, int id, float xmm, float ymm) {
    using TWidget = SwitchWidget<TPanel, TFrame, bool>;
    auto w = rack::createParamCentered<TWidget>(mm2px(xmm, ymm),
                                                panel->getModule(), id);
    BehaviorT::init_behavior(w);
    panel->addParam(w);
  }

  static inline void config(rack::engine::Module *module, int id,
                            std::string const &name, int initial = 0) {
    auto *pq = module->configButton(id, name);
    pq->defaultValue = static_cast<float>(initial);
  }
};
} // namespace dhe
