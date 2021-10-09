#pragma once

#include "dimensions.h"
#include "panel-assets.h"

#include <app/SvgSwitch.hpp>
#include <helpers.hpp>

#include <string>
#include <vector>

namespace dhe {

template <typename PanelT, typename SwitchT>
struct SwitchWidget : public rack::app::SvgSwitch {
  SwitchWidget() {
    auto const panel_prefix = std::string{PanelT::svg_dir} + "/";
    for (auto const &frame_name : SwitchT::frame_names()) {
      addFrame(load_svg(panel_prefix + frame_name));
    }
    shadow->opacity = 0.F;
  }
};

static inline auto stepper_frame_names(std::string const &frame_prefix,
                                       size_t n) -> std::vector<std::string> {
  auto frame_names = std::vector<std::string>{};
  auto const prefix = frame_prefix + "-";
  for (size_t position = 1; position <= n; position++) {
    frame_names.push_back(prefix + std::to_string(position));
  }
  return frame_names;
}

template <int N> struct ThumbSwitch {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("toggle-" + std::to_string(N), N);
    return frame_names;
  }
};

struct Button {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const names =
        std::vector<std::string>{"button-released", "button-pressed"};
    return names;
  }
};

struct OutputButton {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const names = std::vector<std::string>{"output-button-released",
                                                       "output-button-pressed"};
    return names;
  }
};

template <typename PanelT> struct Switches {
  template <typename SwitchT> using Widget = SwitchWidget<PanelT, SwitchT>;

  template <typename SwitchT>
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Widget<SwitchT> * {
    return rack::createParamCentered<Widget<SwitchT>>(mm2px(xmm, ymm), module,
                                                      index);
  }

  template <int N>
  static inline auto thumb(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> Widget<ThumbSwitch<N>> * {
    return create<ThumbSwitch<N>>(module, xmm, ymm, index);
  }

  template <typename ButtonT = Button>
  static inline auto toggle(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Widget<ButtonT> * {
    return create<ButtonT>(module, xmm, ymm, index);
  }

  template <typename ButtonT = Button>
  static inline auto momentary(rack::engine::Module *module, float xmm,
                               float ymm, int index) -> Widget<ButtonT> * {
    auto button = create<ButtonT>(module, xmm, ymm, index);
    button->momentary = true;
    return button;
  }
};

template <typename StepperT> struct Stepper {
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
  static inline void install(PanelT *panel, int id, float xmm, float ymm) {
    panel->addParam(Switches<PanelT>::template create<Stepper<StepperT>>(
        panel->getModule(), xmm, ymm, id));
  }
};
} // namespace dhe
