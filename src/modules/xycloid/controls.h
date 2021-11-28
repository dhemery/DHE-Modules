#pragma once

#include "throb-speed.h"
#include "wobble-ratio.h"

#include "controls/switches.h"
#include "widgets/assets.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {
namespace xycloid {

struct ThrobSpeedKnob {
  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return ThrobSpeed::scale(getValue());
    }

    void setDisplayValue(float display_value) override {
      setValue(ThrobSpeed::normalize(display_value));
    }
  };

  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name, float value = 1.F) -> Quantity * {
    auto const default_value = ThrobSpeed::normalize(value);
    return module->configParam<Quantity>(param_id, 0.F, 1.F, default_value,
                                         name, "x");
  }
};

struct WobbleRatioKnob {
  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return WobbleRatio::scale(getValue(), range_id_, mode_);
    }

    void setDisplayValue(float display_value) override {
      setValue(WobbleRatio::normalize(display_value, range_id_));
    }

    void set_mode(WobbleRatioMode mode) { mode_ = mode; }

    void set_range(WobbleRatioRangeId id) { range_id_ = id; }

  private:
    WobbleRatioMode mode_{WobbleRatioMode::Free};
    WobbleRatioRangeId range_id_{WobbleRatioRangeId::Outward};
  };

  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name, float value = 8.F,
                     WobbleRatioRangeId range = WobbleRatioRangeId::Outward)
      -> Quantity * {
    auto const default_value = WobbleRatio::normalize(value, range);
    auto *q = module->configParam<Quantity>(param_id, 0.F, 1.F, default_value,
                                            name, "x");
    q->set_range(range);
    return q;
  }
};

struct WobbleRatioModeSwitch {
  template <typename Panel>
  struct Widget : ThumbSwitch::Widget<Panel, wobble_ratio::mode_labels.size()> {
  };

  struct Quantity : rack::engine::SwitchQuantity {
    void setValue(float value) override {
      rack::engine::SwitchQuantity::setValue(value);
      auto const mode = static_cast<WobbleRatioMode>(getValue());
      for (auto *knob : knobs_) {
        knob->set_mode(mode);
      }
    }

    void add_knob(WobbleRatioKnob::Quantity *knob) {
      knobs_.push_back(knob);
      knob->set_mode(static_cast<WobbleRatioMode>(getValue()));
    }

  private:
    std::vector<WobbleRatioKnob::Quantity *> knobs_{};
  };

  template <typename Panel>
  static auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    auto *w = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(w);
    return w;
  }
  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name,
                     WobbleRatioMode mode = WobbleRatioMode::Free)
      -> Quantity * {
    static auto const labels = std::vector<std::string>{
        wobble_ratio::mode_labels.cbegin(), wobble_ratio::mode_labels.cend()};
    auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(mode);
    return module->configSwitch<Quantity>(param_id, 0.F, max_value,
                                          default_value, name, labels);
  }
};

struct WobbleRatioRangeSwitch {
  template <typename Panel>
  struct Widget : ThumbSwitch::Widget<Panel, wobble_ratio::ranges.size()> {};

  struct Quantity : rack::engine::SwitchQuantity {
    void setValue(float value) override {
      rack::engine::SwitchQuantity::setValue(value);
      auto const range = static_cast<WobbleRatioRangeId>(getValue());
      for (auto *knob : knobs_) {
        knob->set_range(range);
      }
    }

    void add_knob(WobbleRatioKnob::Quantity *knob) {
      knobs_.push_back(knob);
      knob->set_range(static_cast<WobbleRatioRangeId>(getValue()));
    }

  private:
    std::vector<WobbleRatioKnob::Quantity *> knobs_{};
  };

  template <typename Panel>
  static auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    auto *w = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(w);
    return w;
  }

  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name,
                     WobbleRatioRangeId range = WobbleRatioRangeId::Outward)
      -> Quantity * {
    static auto const labels = std::vector<std::string>{
        wobble_ratio::range_labels.cbegin(), wobble_ratio::range_labels.cend()};
    auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(range);
    return module->configSwitch<Quantity>(param_id, 0.F, max_value,
                                          default_value, name, labels);
  }
};

} // namespace xycloid
} // namespace dhe