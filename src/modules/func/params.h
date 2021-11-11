#pragma once

#include "signals/voltages.h"

#include <string>
#include <vector>

namespace dhe {
namespace func {

struct Offsets {
  enum Selection { Unipolar5, Bipolar, Unipolar, Bipolar10 };
  static constexpr auto stepper_slug = "offset-range";
  static constexpr auto unit = " V";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–5 V", "±5 V", "0–10 V", "±10 V"};
    return labels;
  }

  static inline auto offset(float rotation, Selection selection) -> float {
    return range(selection).scale(rotation);
  }

  static inline auto rotation(float multiplier, Selection selection) -> float {
    return range(selection).normalize(multiplier);
  }

private:
  static inline auto range(Selection selection) -> Range const & {
    static auto constexpr minus_ten_to_plus_ten_range = Range{-10.F, 10.F};
    static auto constexpr zero_to_five_range = Range{0.F, 5.F};
    static auto const ranges = std::vector<Range>{
        zero_to_five_range, BipolarVoltage::range(), UnipolarVoltage::range(),
        minus_ten_to_plus_ten_range};
    return ranges[selection];
  }
};

struct Multipliers {
  enum Selection { Attenuator, Attenuverter, Gain, Gainuverter };
  static constexpr auto stepper_slug = "multiplier-range";
  static constexpr auto unit = "";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–1", "±1", "0–2", "±2"};
    return labels;
  }

  static inline auto multiplier(float rotation, Selection selection) -> float {
    return range(selection).scale(rotation);
  }

  static inline auto rotation(float multiplier, Selection selection) -> float {
    return range(selection).normalize(multiplier);
  }

private:
  static inline auto range(Selection selection) -> Range const & {
    static auto constexpr minus_two_to_plus_two_range = Range{-2.F, 2.F};
    static auto const ranges =
        std::vector<Range>{Attenuator::range(), Attenuverter::range(),
                           Gain::range(), minus_two_to_plus_two_range};
    return ranges[selection];
  }
};

struct Operations {
  using Selection = enum { Add, Multiply };
  static auto constexpr unit = "";

  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels =
        std::vector<std::string>{"Add (offset)", "Multiply (scale)"};
    return labels;
  }

  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return operation_ == Operations::Multiply
                 ? Multipliers::multiplier(rotation, multipler_range_)
                 : Offsets::offset(rotation, offset_range_);
    }

    auto to_rotation(float operand) const -> float {
      return operation_ == Operations::Multiply
                 ? Multipliers::rotation(operand, multipler_range_)
                 : Offsets::rotation(operand, offset_range_);
    }

    void select_operation(Operations::Selection operation) {
      operation_ = operation;
    }

    void select_multiplier_range(Multipliers::Selection selection) {
      multipler_range_ = selection;
    }

    void select_offset_range(Offsets::Selection selection) {
      offset_range_ = selection;
    }

  private:
    Offsets::Selection offset_range_{Offsets::Bipolar};
    Multipliers::Selection multipler_range_{Multipliers::Gain};
    Operations::Selection operation_{Operations::Multiply};
  };
};
} // namespace func
} // namespace dhe
