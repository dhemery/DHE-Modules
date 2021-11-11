#pragma once

#include "assets.h"
#include "dimensions.h"

#include "rack.hpp"

namespace dhe {

// All measurements in mm
struct SelectionMarkerPositions {
  SelectionMarkerPositions(float xmm, float ymm, float dxmm)
      : x_{xmm}, y_{ymm}, dx_{dxmm} {}

  auto start_x() const -> float { return x_ + start_dx; }
  auto end_x() const -> float { return x_ + end_dx; }
  auto dx() const -> float { return dx_; }
  auto y() const -> float { return y_; }

private:
  static auto constexpr start_dx = -2.F * light_diameter;
  static auto constexpr end_dx = 0.F;
  float x_;  // NOLINT
  float y_;  // NOLINT
  float dx_; // NOLINT
};

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
  static inline auto install(TPanel *panel,
                             SelectionMarkerPositions const &positions)
      -> Widget<TPanel> * {
    auto const x = positions.start_x();
    auto const y = positions.y();
    auto const dx = positions.dx();
    auto w = rack::createWidgetCentered<Widget<TPanel>>(mm2px(x, y));
    w->initialize(x, dx);
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
  static inline auto install(TPanel *panel,
                             SelectionMarkerPositions const &positions)
      -> Widget<TPanel> * {
    auto const x = positions.end_x();
    auto const y = positions.y();
    auto const dx = positions.dx();
    auto w = rack::createWidgetCentered<Widget<TPanel>>(mm2px(x, y));
    w->initialize(x, dx);
    panel->addChild(w);
    return w;
  }
};
} // namespace dhe
