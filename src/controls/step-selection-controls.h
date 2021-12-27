#pragma once

#include "knobs.h"

#include "rack.hpp"

#include <string>

namespace dhe {

struct SelectionStartMarker {
  template <typename Panel> struct Widget : public rack::widget::SvgWidget {
    Widget() { setSvg(load_svg(Panel::svg_dir, "marker-start")); }

    void set_start(int index) {
      this->box.pos.x = x_px_ + static_cast<float>(index) * step_width_px_;
    }

    void initialize(float step_width) {
      step_width_px_ = mm2px(step_width);
      // Assumes that curent position is step 1 (index 0)
      x_px_ = this->box.pos.x;
    }

  private:
    float x_px_{};
    float step_width_px_{};
  };

  template <typename P>
  static inline auto install(P *panel, float xmm, float ymm, float step_width)
      -> Widget<P> * {
    auto w = rack::createWidgetCentered<Widget<P>>(mm2px(xmm, ymm));
    w->initialize(step_width);
    panel->addChild(w);
    return w;
  }
};

struct SelectionEndMarker {
  template <typename Panel> struct Widget : public rack::widget::SvgWidget {

    Widget() { setSvg(load_svg(Panel::svg_dir, "marker-end")); }

    void set_start(int index) {
      this->start_index_ = index;
      move();
    }

    void set_length(int length) {
      this->length_ = length;
      move();
    }

    void initialize(float step_width) {
      step_width_px_ = mm2px(step_width);
      // Assumes that curent position is step 1 (index 0)
      x_px_ = this->box.pos.x;
      set_length(Panel::N);
    }

  private:
    static auto constexpr index_mask{Panel::N - 1};

    void move() {
      auto const index = (start_index_ + length_ - 1) & index_mask;
      this->box.pos.x = x_px_ + step_width_px_ * static_cast<float>(index);
    }

    float x_px_{};
    float step_width_px_{};
    int start_index_{0};
    int length_{Panel::N - 1};
  };

  template <typename Panel>
  static inline auto install(Panel *panel, float xmm, float ymm,
                             float step_width) -> Widget<Panel> * {
    auto w = rack::createWidgetCentered<Widget<Panel>>(mm2px(xmm, ymm));
    w->initialize(step_width);
    panel->addChild(w);
    return w;
  }
};

struct SelectionStartKnob {
  using Quantity = rack::engine::ParamQuantity;

  template <typename Panel, typename Style>
  struct Widget : Knob::Widget<Panel, Style> {
    using StartMarker = SelectionStartMarker::Widget<Panel>;
    using EndMarker = SelectionEndMarker::Widget<Panel>;

    Widget() { this->snap = true; }

    // NOLINTNEXTLINE
    void onChange(rack::widget::Widget::ChangeEvent const &e) override {
      rack::app::SvgKnob::onChange(e);
      auto const position =
          static_cast<int>(this->getParamQuantity()->getValue());
      start_marker_->set_start(position);
      end_marker_->set_start(position);
    }

    void set_start_marker(StartMarker *marker) { start_marker_ = marker; }

    void set_end_marker(EndMarker *marker) { end_marker_ = marker; }

  private:
    StartMarker *start_marker_{};
    EndMarker *end_marker_{};
  };

  template <int N>
  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name) -> Quantity * {
    auto const max_value = static_cast<float>(N - 1);
    auto *q = module->configParam<Quantity>(param_id, 0.F, max_value, 0.F, name,
                                            "", 0.F, 1.F, 1.F);
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
    using EndMarker = SelectionEndMarker::Widget<Panel>;

    Widget() { this->snap = true; }

    // NOLINTNEXTLINE
    void onChange(rack::widget::Widget::ChangeEvent const &e) override {
      rack::app::SvgKnob::onChange(e);
      end_marker_->set_length(
          static_cast<int>(this->getParamQuantity()->getValue()));
    }

    void set_end_marker(EndMarker *end_marker) { end_marker_ = end_marker; }

  private:
    EndMarker *end_marker_{};
  };

  template <int N>
  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name) -> Quantity * {
    auto const max_value = static_cast<float>(N);
    auto *q = module->configParam<Quantity>(param_id, 1.F, max_value, max_value,
                                            name, " steps");
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
