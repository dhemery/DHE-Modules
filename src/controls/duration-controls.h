#pragma once

#include "signals/duration-signals.h"
#include "switches.h"
#include "widgets/assets.h"

#include "rack.hpp"

#include <string>

namespace dhe {
struct MediumDurationKnob {
  static auto constexpr default_duration = 1.F;

  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return MediumDuration::scale(getValue());
    }

    void setDisplayValue(float display) override {
      setValue(MediumDuration::normalize(display));
    }
  };

  static auto config(rack::engine::Module *module, int id,
                     std::string const &name, float value = default_duration)
      -> Quantity * {
    auto const default_rotation = MediumDuration::normalize(value);
    auto *q = module->configParam<Quantity>(id, 0.F, 1.F, default_rotation,
                                            name, " s");
    return q;
  }
};

struct DurationKnob {
  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return Duration::scale(getValue(), range_id_);
    }

    void setDisplayValue(float display) override {
      setValue(Duration::normalize(display, range_id_));
    }

    void select_range(DurationRangeId id) { range_id_ = id; }

  private:
    DurationRangeId range_id_{};
  };

  static auto
  config(rack::engine::Module *module, int param_id, std::string const &name,
         float default_duration = MediumDurationKnob::default_duration,
         DurationRangeId default_range_id = DurationRangeId::Medium)
      -> Quantity * {
    auto const default_value =
        Duration::normalize(default_duration, default_range_id);
    auto *q = module->configParam<Quantity>(param_id, 0.F, 1.F, default_value,
                                            name, " s");
    q->select_range(default_range_id);
    return q;
  }
};

struct DurationRangeSwitch {
  template <typename Panel>
  using Widget = ThumbSwitch::Widget<Panel, duration::ranges.size()>;

  struct Quantity : rack::engine::SwitchQuantity {
    void setValue(float value) override {
      rack::engine::SwitchQuantity::setValue(value);
      for (auto *knob : knobs_) {
        knob->select_range(static_cast<DurationRangeId>(value));
      }
    }

    void add_knob(DurationKnob::Quantity *knob) {
      knobs_.push_back(knob);
      knob->select_range(static_cast<DurationRangeId>(getValue()));
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
         DurationRangeId default_range_id = DurationRangeId::Medium)
      -> Quantity * {
    static auto const labels = std::vector<std::string>{
        duration::labels.cbegin(), duration::labels.cend()};
    static auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(default_range_id);
    auto *q = module->configSwitch<Quantity>(param_id, 0.F, max_value,
                                             default_value, name, labels);
    return q;
  }
};
} // namespace dhe