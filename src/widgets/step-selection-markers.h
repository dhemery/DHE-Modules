#pragma once

#include "assets.h"
#include "dimensions.h"

#include "rack.hpp"

namespace dhe {

struct StartMarker {
  template <typename TPanel> struct Widget : public rack::widget::SvgWidget {
    Widget() { setSvg(load_svg(TPanel::svg_dir, "marker-start")); }

    void set_start(int index) {
      auto const xmm = x_ + step_width_ * static_cast<float>(index);
      this->box.pos.x = mm2px(xmm);
    }

    void initialize(float x, float step_width) {
      x_ = x;
      step_width_ = step_width;
      set_start(0);
    }

  private:
    float x_{};
    float step_width_{};
  };

  template <typename TPanel>
  static inline auto install(TPanel *panel, float xmm, float ymm,
                             float step_width) -> Widget<TPanel> * {
    auto w = rack::createWidgetCentered<Widget<TPanel>>(mm2px(xmm, ymm));
    w->initialize(xmm, step_width);
    panel->addChild(w);
    return w;
  }
};

struct EndMarker {
  template <typename TPanel> struct Widget : public rack::widget::SvgWidget {
    Widget() { setSvg(load_svg(TPanel::svg_dir, "marker-end")); }
    static int constexpr index_mask_{TPanel::N - 1};

    void set_start(int index) {
      this->start_index_ = index;
      move();
    }

    void set_length(int length) {
      this->end_offset_ = length - 1;
      move();
    }

    void initialize(float x, float step_width) {
      x_ = x;
      step_width_ = step_width;
      move();
    }

  private:
    void move() {
      auto const index = (start_index_ + end_offset_) & index_mask_;
      auto const xmm = x_ + step_width_ * static_cast<float>(index);
      this->box.pos.x = mm2px(xmm);
    }

    float x_{};
    float step_width_{};
    int start_index_{0};
    int end_offset_{TPanel::N - 1};
  };

  template <typename TPanel>
  static inline auto install(TPanel *panel, float xmm, float ymm,
                             float step_width) -> Widget<TPanel> * {
    auto w = rack::createWidgetCentered<Widget<TPanel>>(mm2px(xmm, ymm));
    w->initialize(xmm, step_width);
    panel->addChild(w);
    return w;
  }
};
} // namespace dhe
