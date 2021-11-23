#pragma once

#include "multiplier.h"
#include "offset.h"

#include <array>
#include <string>
#include <vector>

namespace dhe {
namespace func {
enum class Operation { Add, Multiply };

namespace operations {
static auto constexpr size = 2;
static auto constexpr labels =
    std::array<char const *, size>{"Add (offset)", "Multiply (scale)"};
} // namespace operations

struct Operations {
  using value_type = Operation;
  static auto constexpr size = operations::size;

  static inline auto labels() -> std::vector<std::string> {
    return {operations::labels.cbegin(), operations::labels.cend()};
  }
};

struct Operand {
  struct KnobMap {
    static auto constexpr default_value = 2.5F;
    static auto constexpr unit = "";

    auto to_display(float value) const -> float {
      return operation_ == Operation::Multiply
                 ? multiplier_ranges::scale(value, multipler_range_id_)
                 : offset_ranges::scale(value, offset_range_id_);
    }

    auto to_value(float display) const -> float {
      return operation_ == Operation::Multiply
                 ? multiplier_ranges::normalize(display, multipler_range_id_)
                 : offset_ranges::normalize(display, offset_range_id_);
    }
    void select_operation(Operation operation) { operation_ = operation; }

    void select_multiplier_range(MultiplierRangeId id) {
      multipler_range_id_ = id;
    }

    void select_offset_range(OffsetRangeId id) { offset_range_id_ = id; }

  private:
    OffsetRangeId offset_range_id_{};
    MultiplierRangeId multipler_range_id_{};
    Operation operation_{Operation::Add};
  };
};
} // namespace func
} // namespace dhe
