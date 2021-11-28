#pragma once

#include "app/SvgKnob.hpp"
#include "helpers.hpp"
#include "knobs.h"

#include "rack.hpp"

#include <string>

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

struct SelectionStartMarker {
  template <typename Panel> struct Widget : public rack::widget::SvgWidget {
    Widget() { setSvg(load_svg(Panel::svg_dir, "marker-start")); }

    void initialize(float x, float step_width) {
      x_ = x;
      step_width_ = step_width;
      set_start(0);
    }

    void set_start(int index) {
      auto const xmm = x_ + step_width_ * static_cast<float>(index);
      this->box.pos.x = mm2px(xmm);
    }

  private:
    float x_{};
    float step_width_{};
  };

  template <typename P>
  static inline auto install(P *panel,
                             SelectionMarkerPositions const &positions)
      -> Widget<P> * {
    auto const x = positions.start_x();
    auto const y = positions.y();
    auto const dx = positions.dx();
    auto w = rack::createWidgetCentered<Widget<P>>(mm2px(x, y));
    w->initialize(x, dx);
    panel->addChild(w);
    return w;
  }
};

struct SelectionEndMarker {
  template <typename Panel> struct Widget : public rack::widget::SvgWidget {

    Widget() { setSvg(load_svg(Panel::svg_dir, "marker-end")); }

    void initialize(float x, float step_width) {
      x_ = x;
      step_width_ = step_width;
      move();
    }

    void set_start(int index) {
      this->start_index_ = index;
      move();
    }

    void set_length(int length) {
      this->end_offset_ = length - 1;
      move();
    }

  private:
    static auto constexpr index_mask{Panel::N - 1};

    void move() {
      auto const index = (start_index_ + end_offset_) & index_mask;
      auto const xmm = x_ + step_width_ * static_cast<float>(index);
      this->box.pos.x = mm2px(xmm);
    }

    float x_{};
    float step_width_{};
    int start_index_{0};
    int end_offset_{Panel::N - 1};
  };

  template <typename Panel>
  static inline auto install(Panel *panel,
                             SelectionMarkerPositions const &positions)
      -> Widget<Panel> * {
    auto const x = positions.end_x();
    auto const y = positions.y();
    auto const dx = positions.dx();
    auto w = rack::createWidgetCentered<Widget<Panel>>(mm2px(x, y));
    w->initialize(x, dx);
    panel->addChild(w);
    return w;
  }
};

struct SelectionStartKnob {
  using Quantity = rack::engine::ParamQuantity;

  template <typename Panel, typename Style>
  struct Widget : Knob::Widget<Panel, Style> {
    Widget() { this->snap = true; }

    void onChange(rack::widget::Widget::ChangeEvent const &e) override {
      rack::app::SvgKnob::onChange(e);
      auto const position =
          static_cast<int>(this->getParamQuantity()->getValue());
      start_marker_->set_start(position);
      end_marker_->set_start(position);
    }

    void set_start_marker(SelectionStartMarker::Widget<Panel> *marker) {
      start_marker_ = marker;
    }

    void set_end_marker(SelectionEndMarker::Widget<Panel> *marker) {
      end_marker_ = marker;
    }

  private:
    SelectionStartMarker::Widget<Panel> *start_marker_{};
    SelectionEndMarker::Widget<Panel> *end_marker_{};
  };

  template <int N>
  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name, int value = 1) -> Quantity * {
    auto const max_value = static_cast<float>(N);
    auto const default_value = static_cast<float>(value);
    auto *q = module->configParam<Quantity>(param_id, 1.F, max_value,
                                            default_value, name);
    q->snapEnabled = true;
    return q;
  }

  template <typename Style, typename Panel>
  static auto install(Panel *panel, int param_id, float xmm, float ymm,
                      SelectionStartMarker::Widget<Panel> *start_marker,
                      SelectionEndMarker::Widget<Panel> *end_marker)
      -> Widget<Panel, Style> * {
    auto *w = rack::createParamCentered<Widget<Panel, Style>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    w->set_start_marker(start_marker);
    w->set_end_marker(end_marker);
    panel->addParam(w);
    return w;
  }
};

struct SelectionLengthKnob {
  using Quantity = rack::engine::ParamQuantity;

  template <typename Panel, typename Style>
  struct Widget : Knob::Widget<Panel, Style> {
    Widget() { this->snap = true; }

    void onChange(rack::widget::Widget::ChangeEvent const &e) override {
      rack::app::SvgKnob::onChange(e);
      end_marker_->set_length(
          static_cast<int>(this->getParamQuantity()->getValue()));
    }

    void set_end_marker(SelectionEndMarker::Widget<Panel> *end_marker) {
      end_marker_ = end_marker;
    }

  private:
    SelectionEndMarker::Widget<Panel> *end_marker_{};
  };

  template <int N>
  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name, int value = 1) -> Quantity * {
    auto const max_value = static_cast<float>(N - 1);
    auto const default_value = static_cast<float>(value);
    auto *q = module->configParam<Quantity>(param_id, 0.F, max_value,
                                            default_value, name);
    q->snapEnabled = true;
    return q;
  }

  template <typename Style, typename Panel>
  static auto install(Panel *panel, int param_id, float xmm, float ymm,
                      SelectionEndMarker::Widget<Panel> *end_marker)
      -> Widget<Panel, Style> * {
    auto *w = rack::createParamCentered<Widget<Panel, Style>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    w->set_end_marker(end_marker);
    panel->addParam(w);
    return w;
  }
};

} // namespace dhe
