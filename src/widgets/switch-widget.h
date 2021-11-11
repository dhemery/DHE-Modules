#pragma once
#include "assets.h"

#include "rack.hpp"

#include <functional>
#include <string>
#include <vector>

namespace dhe {
template <typename TPanel, typename TStyle, typename TItems>
struct SwitchWidget : public rack::app::SvgSwitch {
  using Selection = typename TItems::Selection;
  using Action = std::function<void(Selection)>;

  SwitchWidget() {
    auto const panel_prefix = std::string{TPanel::svg_dir} + "/";
    for (auto const &file_name : file_names()) {
      addFrame(load_svg(panel_prefix + file_name));
    }
    shadow->opacity = 0.F;
  }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgSwitch::onChange(e);
    auto const selection =
        static_cast<Selection>(this->getParamQuantity()->getValue());
    action_(selection);
  }

  void on_change(Action const &action) { action_ = action; }

private:
  static inline auto file_names() -> std::vector<std::string> const & {
    static auto frame_names = std::vector<std::string>{};
    auto const prefix = TStyle::slug() + "-";
    auto const size = TItems::labels().size();
    for (size_t position = 1; position <= size; position++) {
      frame_names.push_back(prefix + std::to_string(position));
    }
    return frame_names;
  }

  Action action_ = [](Selection) {};
};

} // namespace dhe
