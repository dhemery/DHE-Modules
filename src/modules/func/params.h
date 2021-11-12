#pragma once

#include "signals/voltages.h"

#include <string>
#include <vector>

namespace dhe {
namespace func {

enum class OffsetRange { Unipolar5, Bipolar, Unipolar, Bipolar10 };

struct OffsetRanges {
  using Selection = OffsetRange;
  static constexpr auto stepper_slug = "offset-range";
  static constexpr auto unit = " V";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–5 V", "±5 V", "0–10 V", "±10 V"};
    return labels;
  }

  static inline auto offset(float rotation, OffsetRange offset) -> float {
    return select(offset).scale(rotation);
  }

  static inline auto rotation(float multiplier, OffsetRange offset) -> float {
    return select(offset).normalize(multiplier);
  }

private:
  static inline auto select(OffsetRange offset) -> Range const & {
    static auto constexpr minus_ten_to_plus_ten_range = Range{-10.F, 10.F};
    static auto constexpr zero_to_five_range = Range{0.F, 5.F};
    static auto const ranges = std::vector<Range>{
        zero_to_five_range, BipolarVoltage::range(), UnipolarVoltage::range(),
        minus_ten_to_plus_ten_range};
    return ranges[(int)offset];
  }
};

enum class MultiplierRange { Attenuator, Attenuverter, Gain, Gainuverter };

struct MultiplierRanges {
  using Selection = MultiplierRange;
  static constexpr auto stepper_slug = "multiplier-select";
  static constexpr auto unit = "";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–1", "±1", "0–2", "±2"};
    return labels;
  }

  static inline auto multiplier(float rotation, MultiplierRange range)
      -> float {
    return select_range(range).scale(rotation);
  }

  static inline auto rotation(float multiplier, MultiplierRange range)
      -> float {
    return select_range(range).normalize(multiplier);
  }

private:
  static inline auto select_range(Selection selection) -> Range const & {
    static auto constexpr minus_two_to_plus_two_range = Range{-2.F, 2.F};
    static auto const ranges =
        std::vector<Range>{Attenuator::range(), Attenuverter::range(),
                           Gain::range(), minus_two_to_plus_two_range};
    return ranges[(int)selection];
  }
};

enum class Operation { Add, Multiply };

struct Operations {
  struct KnobMapper;
  using Selection = Operation;
  static auto constexpr unit = "";

  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels =
        std::vector<std::string>{"Add (offset)", "Multiply (scale)"};
    return labels;
  }
};

struct Operations::KnobMapper {
  auto to_display_value(float rotation) const -> float {
    return operation_ == Operation::Multiply
               ? MultiplierRanges::multiplier(rotation, multipler_range_)
               : OffsetRanges::offset(rotation, offset_range_);
  }

  auto to_rotation(float operand) const -> float {
    return operation_ == Operation::Multiply
               ? MultiplierRanges::rotation(operand, multipler_range_)
               : OffsetRanges::rotation(operand, offset_range_);
  }

  void select_operation(Operation operation) { operation_ = operation; }

  void select_multiplier_range(MultiplierRange range) {
    multipler_range_ = range;
  }

  void select_offset_range(OffsetRange range) { offset_range_ = range; }

private:
  OffsetRange offset_range_{OffsetRange::Bipolar};
  MultiplierRange multipler_range_{MultiplierRange::Gain};
  Operation operation_{Operation::Multiply};
};
} // namespace func
} // namespace dhe
