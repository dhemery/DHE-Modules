#pragma once

#include "signals/voltages.h"

#include <string>
#include <vector>

namespace dhe {
namespace func {

enum class OffsetRangeId { Unipolar5, Bipolar, Unipolar, Bipolar10 };

struct OffsetRanges {
  using Selection = OffsetRangeId;
  static constexpr auto stepper_slug = "offset-range";
  static constexpr auto unit = " V";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–5 V", "±5 V", "0–10 V", "±10 V"};
    return labels;
  }

  static inline auto scale(float normalized, OffsetRangeId range_id) -> float {
    return range(range_id).scale(normalized);
  }

  static inline auto normalize(float scaled, OffsetRangeId range_id) -> float {
    return range(range_id).normalize(scaled);
  }

private:
  static inline auto range(OffsetRangeId id) -> Range const & {
    static auto constexpr minus_ten_to_plus_ten_range = Range{-10.F, 10.F};
    static auto constexpr zero_to_five_range = Range{0.F, 5.F};
    static auto const ranges = std::vector<Range>{
        zero_to_five_range, BipolarVoltage::range(), UnipolarVoltage::range(),
        minus_ten_to_plus_ten_range};
    return ranges[static_cast<int>(id)];
  }
};

enum class MultiplierRangeId { Attenuator, Attenuverter, Gain, Gainuverter };

struct MultiplierRanges {
  using Selection = MultiplierRangeId;
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

private:
  static inline auto range(MultiplierRangeId id) -> Range const & {
    static auto constexpr minus_two_to_plus_two_range = Range{-2.F, 2.F};
    static auto const ranges =
        std::vector<Range>{Attenuator::range(), Attenuverter::range(),
                           Gain::range(), minus_two_to_plus_two_range};
    return ranges[static_cast<int>(id)];
  }
};

enum class Operation { Add, Multiply };

struct Operations {
  struct KnobMapper;
  using Selection = Operation;
  static auto constexpr default_rotation = 0.5F;
  static auto constexpr unit = "";

  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels =
        std::vector<std::string>{"Add (offset)", "Multiply (scale)"};
    return labels;
  }
};

struct Operations::KnobMapper {
  auto scale(float normalized) const -> float {
    return operation_ == Operation::Multiply
               ? MultiplierRanges::scale(normalized, multipler_range_id_)
               : OffsetRanges::scale(normalized, offset_range_id_);
  }

  auto normalize(float scaled) const -> float {
    return operation_ == Operation::Multiply
               ? MultiplierRanges::normalize(scaled, multipler_range_id_)
               : OffsetRanges::normalize(scaled, offset_range_id_);
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
