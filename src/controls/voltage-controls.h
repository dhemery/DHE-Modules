#pragma once

#include "knobs.h"
#include "signals/voltage-signals.h"
#include "switches.h"

#include "rack.hpp"

namespace dhe {

struct BipolarKnob : LinearKnob<BipolarKnob> {
  static auto constexpr default_value = 0.F;
  static auto constexpr &range = BipolarVoltage::range;
  static auto constexpr unit = voltage::unit;
};

struct UnipolarKnob : LinearKnob<UnipolarKnob> {
  static auto constexpr default_value = 5.F;
  static auto constexpr &range = UnipolarVoltage::range;
  static auto constexpr unit = voltage::unit;
};

struct RescalableKnobQuantity : rack::engine::ParamQuantity {
  void set_range(VoltageRangeId id) {
    auto const range = Voltage::range(id);
    displayOffset = range.lower_bound();
    displayMultiplier = range.size();
  }
};

struct VoltageKnob {
  struct Quantity : RescalableKnobQuantity {};

  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name,
                     float voltage = BipolarKnob::default_value,
                     VoltageRangeId range = VoltageRangeId::Bipolar)
      -> Quantity * {
    auto const default_value = Voltage::normalize(voltage, range);
    auto *q = module->configParam<Quantity>(param_id, 0.F, 1.F, default_value,
                                            name, voltage::unit);
    q->set_range(range);
    return q;
  }
};

struct VoltageRangeSwitch {
  template <typename Panel>
  using Widget = ThumbSwitch::Widget<Panel, voltage::ranges.size()>;

  struct Quantity : rack::engine::SwitchQuantity {
    void setValue(float value) override {
      rack::engine::SwitchQuantity::setValue(value);
      for (auto *knob : knobs_) {
        knob->set_range(static_cast<VoltageRangeId>(value));
      }
    }

    void add_knob(VoltageKnob::Quantity *knob) {
      knobs_.push_back(knob);
      knob->set_range(static_cast<VoltageRangeId>(getValue()));
    }

  private:
    std::vector<VoltageKnob::Quantity *> knobs_{};
  };

  template <typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    auto *w = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(w);
    return w;
  }

  static inline auto config(rack::engine::Module *module, int param_id,
                            std::string const &name,
                            VoltageRangeId range = VoltageRangeId::Bipolar)
      -> Quantity * {
    static auto const labels = std::vector<std::string>{
        voltage::labels.cbegin(), voltage::labels.cend()};
    static auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(range);
    auto *q = module->configSwitch<Quantity>(param_id, 0.F, max_value,
                                             default_value, name, labels);
    return q;
  }
};

} // namespace dhe
