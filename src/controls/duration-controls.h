#pragma once

#include "controls/switches.h"

#include "panels/assets.h"
#include "signals/duration.h"

#include "rack.hpp"

#include <string>

namespace dhe {
struct MediumDurationKnob {
  static auto constexpr default_duration = 1.F;

  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return medium_duration_curve.scale(getValue());
    }

    void setDisplayValue(float display) override {
      setValue(medium_duration_curve.normalize(display));
    }
  };

  static auto config(rack::engine::Module *module, int id,
                     std::string const &name, float value = default_duration)
      -> Quantity * {
    auto const default_rotation = medium_duration_curve.normalize(value);
    auto *q = module->configParam<Quantity>(id, 0.F, 1.F, default_rotation,
                                            name, " s");
    return q;
  }
};

struct DurationKnob {
  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return DurationCurve::by_id(range_id_).scale(getValue());
    }

    void setDisplayValue(float display) override {
      setValue(DurationCurve::by_id(range_id_).normalize(display));
    }

    void select_range(DurationCurve::Id id) { range_id_ = id; }

  private:
    DurationCurve::Id range_id_{};
  };

  static auto
  config(rack::engine::Module *module, int param_id, std::string const &name,
         float default_duration = MediumDurationKnob::default_duration,
         DurationCurve::Id default_range_id = DurationCurve::Id::Medium)
      -> Quantity * {
    auto const default_value =
        DurationCurve::by_id(default_range_id).normalize(default_duration);
    auto *q = module->configParam<Quantity>(param_id, 0.F, 1.F, default_value,
                                            name, " s");
    q->select_range(default_range_id);
    return q;
  }
};

struct DurationRangeSwitch {
  template <typename Panel>
  using Widget = ThumbSwitch::Widget<Panel, DurationCurve::count>;

  struct Quantity : rack::engine::SwitchQuantity {
    void setValue(float value) override {
      rack::engine::SwitchQuantity::setValue(value);
      for (auto *knob : knobs_) {
        knob->select_range(static_cast<DurationCurve::Id>(value));
      }
    }

    void add_knob(DurationKnob::Quantity *knob) {
      knobs_.push_back(knob);
      knob->select_range(static_cast<DurationCurve::Id>(getValue()));
    }

  private:
    std::vector<DurationKnob::Quantity *> knobs_{};
  };

  template <typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    auto *w = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(w);
    return w;
  }

  static inline auto
  config(rack::engine::Module *module, int param_id, std::string const &name,
         DurationCurve::Id default_range_id = DurationCurve::Id::Medium)
      -> Quantity * {
    static auto const labels = std::vector<std::string>{
        DurationCurve::labels().cbegin(), DurationCurve::labels().cend()};
    static auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(default_range_id);
    auto *q = module->configSwitch<Quantity>(param_id, 0.F, max_value,
                                             default_value, name, labels);
    return q;
  }
};
} // namespace dhe
