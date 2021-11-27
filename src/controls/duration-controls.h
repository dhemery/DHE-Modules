#pragma once
#include "signals/duration-signals.h"

#include "rack.hpp"

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
    auto const rotation =
        Duration::normalize(default_duration, default_range_id);
    auto *q =
        module->configParam<Quantity>(param_id, 0.F, 1.F, rotation, name, " s");
    q->select_range(default_range_id);
    return q;
  }
};
} // namespace dhe