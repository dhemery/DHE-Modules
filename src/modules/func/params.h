#pragma once

#include "signals/enums.h"
#include "signals/voltages.h"

#include <string>
#include <vector>

namespace dhe {
namespace func {

enum class OffsetRangeId { Unipolar5, Bipolar, Unipolar, Bipolar10 };

struct OffsetRanges : Enums<OffsetRangeId, 4> {
  static constexpr auto stepper_slug = "offset-range";
  static constexpr auto unit = voltage_unit;

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "0–5 V", BipolarVoltage::label, UnipolarVoltage::label, "±10 V"};
    return labels;
  }

  static inline auto scale(float normalized, OffsetRangeId range_id) -> float {
    return range(range_id).scale(normalized);
  }

  static inline auto normalize(float scaled, OffsetRangeId range_id) -> float {
    return range(range_id).normalize(scaled);
  }

  static inline auto range(OffsetRangeId id) -> Range {
    return ranges()[static_cast<int>(id)];
  }

  static inline auto ranges() -> std::vector<Range> const & {
    static auto constexpr minus_ten_to_plus_ten_range = Range{-10.F, 10.F};
    static auto constexpr zero_to_five_range = Range{0.F, 5.F};
    static auto const ranges = std::vector<Range>{
        zero_to_five_range, BipolarVoltage::range(), UnipolarVoltage::range(),
        minus_ten_to_plus_ten_range};
    return ranges;
  }
};

enum class MultiplierRangeId { Attenuator, Attenuverter, Gain, Gainuverter };

struct MultiplierRanges : Enums<MultiplierRangeId, 4> {
  static constexpr auto stepper_slug = "multiplier-range";
  static constexpr auto unit = "";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–1", "±1", "0–2", "±2"};
    return labels;
  }

  static inline auto scale(float normalized, MultiplierRangeId range_id)
      -> float {
    return range(range_id).scale(normalized);
  }

  static inline auto normalize(float scaled, MultiplierRangeId range_id)
      -> float {
    return range(range_id).normalize(scaled);
  }

  static inline auto range(MultiplierRangeId id) -> Range {
    return ranges()[static_cast<int>(id)];
  }

  static inline auto ranges() -> std::vector<Range> const & {
    static auto constexpr minus_two_to_plus_two_range = Range{-2.F, 2.F};
    static auto const ranges =
        std::vector<Range>{Attenuator::range(), Attenuverter::range(),
                           Gain::range(), minus_two_to_plus_two_range};
    return ranges;
  }
};

enum class Operation { Add, Multiply };

struct Operations : Enums<Operation, 2> {
  struct KnobMap;

  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels =
        std::vector<std::string>{"Add (offset)", "Multiply (scale)"};
    return labels;
  }
};

struct Operations::KnobMap {
  static auto constexpr default_rotation = 0.5F;
  static auto constexpr unit = "";

  auto to_display(float value) const -> float {
    return operation_ == Operation::Multiply
               ? MultiplierRanges::scale(value, multipler_range_id_)
               : OffsetRanges::scale(value, offset_range_id_);
  }

  auto to_value(float display) const -> float {
    return operation_ == Operation::Multiply
               ? MultiplierRanges::normalize(display, multipler_range_id_)
               : OffsetRanges::normalize(display, offset_range_id_);
  }

  void select_operation(Operation operation) { operation_ = operation; }

  void select_multiplier_range(MultiplierRangeId id) {
    multipler_range_id_ = id;
  }

  void select_offset_range(OffsetRangeId id) { offset_range_id_ = id; }

private:
  OffsetRangeId offset_range_id_{};
  MultiplierRangeId multipler_range_id_{};
  Operation operation_{};
};
} // namespace func
} // namespace dhe
