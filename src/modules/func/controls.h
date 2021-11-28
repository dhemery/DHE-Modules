#pragma once

#include "engine/ParamQuantity.hpp"
#include "helpers.hpp"
#include "multiplier.h"
#include "offset.h"
#include "operation.h"

#include "controls/knobs.h"
#include "controls/switches.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {
namespace func {
struct OperandKnob {
  template <typename Panel> struct Widget : Knob::Widget<Panel, Large> {};

  struct Quantity : rack::engine::ParamQuantity {
    static auto constexpr default_value = BipolarVoltage::scale(0.F);

    auto getDisplayValue() -> float override {
      auto const value = getValue();
      return operation_ == Operation::Multiply
                 ? Multiplier::scale(value, multipler_range_id_)
                 : Offset::scale(value, offset_range_id_);
    }

    void setDisplayValue(float display_value) override {
      auto const value =
          operation_ == Operation::Multiply
              ? Multiplier::normalize(display_value, multipler_range_id_)
              : Offset::normalize(display_value, offset_range_id_);
      setValue(value);
    }

    auto getLabel() -> std::string override {
      return operation_ == Operation::Multiply ? multiplier_knob_name_
                                               : offset_knob_name_;
    }

    auto getUnit() -> std::string override {
      return operation_ == Operation::Multiply ? "" : voltage::unit;
    }

    auto set_multiplier_knob_name(std::string const &name) -> Quantity * {
      multiplier_knob_name_ = name;
      return this;
    }

    void set_multiplier_range(MultiplierRangeId id) {
      multipler_range_id_ = id;
    }

    auto set_offset_knob_name(std::string const &name) -> Quantity * {
      offset_knob_name_ = name;
      return this;
    }

    void set_offset_range(OffsetRangeId id) { offset_range_id_ = id; }

    void set_operation(Operation operation) { operation_ = operation; }

  private:
    std::string multiplier_knob_name_{};
    MultiplierRangeId multipler_range_id_{MultiplierRangeId::Gain};
    std::string offset_knob_name_{};
    OffsetRangeId offset_range_id_{OffsetRangeId::Bipolar};
    Operation operation_{Operation::Add};
  };

  template <typename Style, typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    auto *w = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(w);
    return w;
  }

  static inline auto config(rack::engine::Module *module, int param_id)
      -> Quantity * {
    return module->configParam<Quantity>(param_id, 0.F, 1.F, 0.5F);
  }
};

struct OffsetRangeStepper {
  static auto constexpr size = offsets::labels.size();
  static auto constexpr slug = "offset-range";

  template <typename Panel>
  struct Widget : Stepper::Widget<Panel, OffsetRangeStepper> {};

  struct Quantity : rack::engine::SwitchQuantity {
    void setValue(float value) override {
      rack::engine::SwitchQuantity::setValue(value);
      operand_knob_->set_offset_range(static_cast<OffsetRangeId>(getValue()));
    }

    auto set_operand_knob(OperandKnob::Quantity *knob) -> Quantity * {
      operand_knob_ = knob;
      return this;
    }

  private:
    OperandKnob::Quantity *operand_knob_{};
  };

  template <typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm,
                             OperandKnob::Widget<Panel> *knob)
      -> Widget<Panel> * {
    auto *w = rack::createParamCentered<Widget<Panel>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(w);
    return w;
  }

  static inline auto config(rack::engine::Module *module, int param_id,
                            std::string const &name) -> Quantity * {
    auto const labels = std::vector<std::string>(offsets::labels.cbegin(),
                                                 offsets::labels.cend());
    auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(OffsetRangeId::Bipolar);
    return module->configSwitch<Quantity>(param_id, 0.F, max_value,
                                          default_value, name, labels);
  }
};

struct MultiplierRangeStepper {
  static auto constexpr size = offsets::labels.size();
  static auto constexpr slug = "multiplier-range";

  template <typename Panel>
  struct Widget : Stepper::Widget<Panel, MultiplierRangeStepper> {};
  struct Quantity : rack::engine::SwitchQuantity {
    void setValue(float value) override {
      rack::engine::SwitchQuantity::setValue(value);
      operand_knob_->set_multiplier_range(
          static_cast<MultiplierRangeId>(getValue()));
    }

    auto set_operand_knob(OperandKnob::Quantity *knob) -> Quantity * {
      operand_knob_ = knob;
      return this;
    }

  private:
    OperandKnob::Quantity *operand_knob_{};
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
                            std::string const &name) -> Quantity * {
    auto const labels = std::vector<std::string>(multipliers::labels.cbegin(),
                                                 multipliers::labels.cend());
    auto const max_value = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(MultiplierRangeId::Gain);
    return module->configSwitch<Quantity>(param_id, 0.F, max_value,
                                          default_value, name, labels);
  }
};

struct OperationSwitch {
  template <typename Panel> struct Widget : ThumbSwitch::Widget<Panel, 2> {
    using MultiplierWidget = MultiplierRangeStepper::Widget<Panel>;
    using OffsetWidget = OffsetRangeStepper::Widget<Panel>;

    void onChange(rack::app::ParamWidget::ChangeEvent const &e) override {
      ThumbSwitch::Widget<Panel, 2>::onChange(e);
      auto const operation =
          static_cast<Operation>(this->getParamQuantity()->getValue());
      if (operation == Operation::Multiply) {
        multiplier_range_stepper_->setVisible(true);
        offset_range_stepper_->setVisible(false);
      } else {
        offset_range_stepper_->setVisible(true);
        multiplier_range_stepper_->setVisible(false);
      }
    }

    auto set_multiplier_range_stepper(MultiplierWidget *stepper) -> Widget * {
      multiplier_range_stepper_ = stepper;
      return this;
    }

    auto set_offset_range_stepper(OffsetWidget *stepper) -> Widget * {
      offset_range_stepper_ = stepper;
      return this;
    }

  private:
    MultiplierWidget *multiplier_range_stepper_{};
    OffsetWidget *offset_range_stepper_{};
  };

  struct Quantity : rack::engine::SwitchQuantity {
    void setValue(float value) override {
      rack::engine::SwitchQuantity::setValue(value);
      operand_knob_->set_operation(static_cast<Operation>(getValue()));
    }

    auto set_operand_knob(OperandKnob::Quantity *knob) -> Quantity * {
      operand_knob_ = knob;
      return this;
    }

  private:
    OperandKnob::Quantity *operand_knob_{};
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
                            std::string const &name) -> Quantity * {
    auto const labels = std::vector<std::string>{operations::labels.cbegin(),
                                                 operations::labels.cend()};
    auto const default_value = static_cast<float>(Operation::Add);
    auto const max_value = static_cast<float>(labels.size() - 1);
    return module->configSwitch<Quantity>(param_id, 0.F, max_value,
                                          default_value, name, labels);
  }
};
} // namespace func
} // namespace dhe