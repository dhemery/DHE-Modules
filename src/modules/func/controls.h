#pragma once

#include "app/SvgSwitch.hpp"
#include "multiplier.h"
#include "offset.h"
#include "operation.h"

#include "controls/knobs.h"
#include "controls/switches.h"

#include "rack.hpp"

namespace dhe {
namespace func {
struct OperandKnob {
  template <typename Panel> struct Widget : Knob::Widget<Panel, Small> {};

  struct Quantity : rack::engine::ParamQuantity {
    static auto constexpr default_value = BipolarVoltage::scale(0.F);
    static auto constexpr unit = "";

    auto to_display(float value) const -> float {
      return operation_ == Operation::Multiply
                 ? Multiplier::scale(value, multipler_range_id_)
                 : Offset::scale(value, offset_range_id_);
    }

    auto to_value(float display) const -> float {
      return operation_ == Operation::Multiply
                 ? Multiplier::normalize(display, multipler_range_id_)
                 : Offset::normalize(display, offset_range_id_);
    }

    void set_operation(Operation operation) { operation_ = operation; }

    void set_multiplier_range(MultiplierRangeId id) {
      multipler_range_id_ = id;
    }

    void set_offset_range(OffsetRangeId id) { offset_range_id_ = id; }

  private:
    OffsetRangeId offset_range_id_{OffsetRangeId::Bipolar};
    MultiplierRangeId multipler_range_id_{MultiplierRangeId::Gain};
    Operation operation_{Operation::Add};
  };

  template <typename Style, typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel> * {
    return nullptr;
  }

  static inline auto config(rack::engine::Module *module, int param_id,
                            std::string const &name) -> Quantity * {
    return nullptr;
  }
};

struct OffsetRangeStepper {
  static auto constexpr &labels = offsets::labels;
  static auto constexpr size = labels.size();
  static auto constexpr slug = "offset-range";
  static auto constexpr unit = voltage::unit;

  template <typename Panel>
  struct Widget : StepperWidget<Panel, OffsetRangeStepper> {};
  struct Quantity : rack::engine::SwitchQuantity {};

  static inline auto config(rack::engine::Module *module, int param_id,
                            std::string const &name,
                            OperandKnob::Quantity *knob) -> Quantity * {
    return nullptr;
  }

  template <typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm,
                             OperandKnob::Widget<Panel> *knob)
      -> Widget<Panel> * {
    return nullptr;
  }
};

struct MultiplierRangeStepper {
  static auto constexpr &labels = multipliers::labels;
  static auto constexpr size = labels.size();
  static auto constexpr slug = "multiplier-range";
  static auto constexpr unit = "";

  template <typename Panel>
  struct Widget : StepperWidget<Panel, MultiplierRangeStepper> {};
  struct Quantity : rack::engine::SwitchQuantity {};

  static inline auto config(rack::engine::Module *module, int param_id,
                            std::string const &name,
                            OperandKnob::Quantity *knob) -> Quantity * {
    return nullptr;
  }

  template <typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm,
                             OperandKnob::Widget<Panel> *knob)
      -> Widget<Panel> * {
    return nullptr;
  }
};

struct OperationSwitch {
  template <typename Panel> using Widget = ThumbSwitch::Widget<Panel, 2>;

  struct Quantity : rack::engine::SwitchQuantity {
  private:
    OperandKnob::Quantity *operand_knob_{};
  };

  static inline auto
  config(rack::engine::Module *module, int param_id, std::string const &name,
         OffsetRangeStepper::Quantity *offset_range_stepper,
         MultiplierRangeStepper::Quantity *multiplier_range_stepper)
      -> Quantity * {
    return nullptr;
  }

  template <typename Panel>
  static inline auto
  install(Panel *panel, int param_id, float xmm, float ymm,
          OffsetRangeStepper::Widget<Panel> *offset_range_stepper,
          MultiplierRangeStepper::Widget<Panel> *multiplier_range_stepper)
      -> Widget<Panel> * {
    return nullptr;
  }
};
} // namespace func
} // namespace dhe