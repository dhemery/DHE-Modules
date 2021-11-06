#pragma once

#include "dimensions.h"

namespace dhe {
struct StartMarker {
  template <typename TPanel> struct Widget : public rack::widget::SvgWidget {
    Widget() { setSvg(load_svg(TPanel::svg_dir, "marker-start")); }

    void set_selection_start(int step) {
      auto constexpr base_x = TPanel::step_x - 2.F * light_diameter;
      auto const x = base_x + TPanel::step_dx * static_cast<float>(step);
      this->box.pos.x = mm2px(x);
    }
  };

  template <typename TPanel>
  static inline auto install(TPanel *panel, float xmm, float ymm)
      -> Widget<TPanel> * {
    auto w = rack::createWidgetCentered<Widget<TPanel>>(mm2px(xmm, ymm));
    panel->addChild(w);
    return w;
  }
};

struct EndMarker {
  template <typename TPanel> struct Widget : public rack::widget::SvgWidget {
    Widget() { setSvg(load_svg(TPanel::svg_dir, "marker-end")); }

    void set_selection_start(int step) {
      this->selection_start_ = step;
      move();
    }
    void set_selection_length(int length) {
      this->selection_length_ = length;
      move();
    }

  private:
    void move() {
      auto const selection_end =
          (selection_start_ + selection_length_ - 1) & step_mask_;
      auto const x =
          TPanel::step_x + TPanel::step_dx * static_cast<float>(selection_end);
      this->box.pos.x = mm2px(x);
    }

    int selection_start_{};
    int selection_length_{};
    int const step_mask_ = TPanel::N - 1;
  };

  template <typename TPanel>
  static inline auto install(TPanel *panel, float xmm, float ymm)
      -> Widget<TPanel> * {
    auto w = rack::createWidgetCentered<Widget<TPanel>>(mm2px(xmm, ymm));
    panel->addChild(w);
    return w;
  }
};
} // namespace dhe
