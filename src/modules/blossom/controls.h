#pragma once

#include "bounce-ratio.h"
#include "spin-speed.h"

#include "controls/switches.h"
#include "panels/assets.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {
namespace blossom {
struct BounceRatioKnob {
  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return BounceRatio::scale(getValue(), mode_);
    }

    void setDisplayValue(float display_value) override {
      setValue(BounceRatio::normalize(display_value));
    }

    void set_mode(BounceRatioMode mode) { mode_ = mode; }

  private:
    BounceRatioMode mode_{};
  };

  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name, float value = 9.F) -> Quantity * {
    auto const default_value = BounceRatio::normalize(value);
    return module->configParam<Quantity>(param_id, 0.F, 1.F, default_value,
                                         name, "x");
  }
};

struct BounceRatioModeSwitch {
  template <typename Panel>
  struct Widget : ThumbSwitch::Widget<Panel, bounce_ratio::size> {};

  struct Quantity : rack::engine::SwitchQuantity {
    void setValue(float value) override {
      rack::engine::SwitchQuantity::setValue(value);
      auto const mode = static_cast<BounceRatioMode>(getValue());
      for (auto *knob : knobs_) {
        knob->set_mode(mode);
      }
    }

    void add_knob(BounceRatioKnob::Quantity *knob) {
      knobs_.push_back(knob);
      knob->set_mode(static_cast<BounceRatioMode>(getValue()));
    }

  private:
    std::vector<BounceRatioKnob::Quantity *> knobs_{};
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
                     BounceRatioMode mode = BounceRatioMode::Free)
      -> Quantity * {
    static auto const labels = std::vector<std::string>{
        bounce_ratio::labels.cbegin(), bounce_ratio::labels.cend()};
    auto const max_value = static_cast<float>(bounce_ratio::size - 1);
    auto const default_value = static_cast<float>(mode);
    return module->configSwitch<Quantity>(param_id, 0.F, max_value,
                                          default_value, name, labels);
  }
};

struct SpinSpeedKnob {
  struct Quantity : rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return SpinSpeed::scale(getValue());
    }

    void setDisplayValue(float display_value) override {
      setValue(SpinSpeed::normalize(display_value));
    }
  };

  static auto config(rack::engine::Module *module, int param_id,
                     std::string const &name, float value = 1.F) -> Quantity * {
    auto const default_value = SpinSpeed::normalize(value);
    return module->configParam<Quantity>(param_id, 0.F, 1.F, default_value,
                                         name, "x");
  }
};

} // namespace blossom
} // namespace dhe