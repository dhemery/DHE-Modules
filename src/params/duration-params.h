#pragma once
#include "signals/duration-signals.h"

#include "rack.hpp"

namespace dhe {
struct MediumDurationKnob : rack::engine::ParamQuantity {
  static auto constexpr default_value = 1.F;
  static auto config(rack::engine::Module *module, int id,
                     std::string const &name, float value = default_value)
      -> MediumDurationKnob * {
    auto const default_rotation = MediumDuration::normalize(value);
    auto *q = module->configParam<MediumDurationKnob>(
        id, 0.F, 1.F, default_rotation, name, " s");
    return q;
  }

  auto getDisplayValue() -> float override {
    return MediumDuration::scale(getValue());
  }

  void setDisplayValue(float display) override {
    setValue(MediumDuration::normalize(display));
  }
};

struct DurationKnob : rack::engine::ParamQuantity {
  static auto config(rack::engine::Module *module, int id,
                     std::string const &name,
                     float value = MediumDurationKnob::default_value,
                     DurationRangeId range_id = DurationRangeId::Medium)
      -> DurationKnob * {
    auto const default_rotation = Duration::normalize(value, range_id);
    auto *q = module->configParam<DurationKnob>(id, 0.F, 1.F, default_rotation,
                                                name, " s");
    q->select_range(range_id);
    return q;
  }

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
} // namespace dhe