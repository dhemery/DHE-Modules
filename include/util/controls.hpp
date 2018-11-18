#pragma once

#include <functional>

namespace DHE {
namespace Controls {

/**
 * Creates a function that returns the value of the source.
 * @param source the source of the value
 * @return a function that returns the value of the source
 */
template<typename Source>
auto float_value_of(const Source &source) -> std::function<float()> {
  return [&source]() -> float {
    return source.value;
  };
}

/**
 * Creates a function that returns {@code true} iff the value of the source exceeds 0.1f.
 * @param source the source of the value
 * @return a function that returns {@code true} iff the value of the source exceeds 0.1f
 */
template<typename Source>
auto bool_value_of(const Source &source) -> std::function<bool()> {
  return [&source]() -> float {
    return source.value > 0.5f;
  };
}

/**
 * Creates a function that converts the value of the source to the nearest {@code int}.
 * @param source the source of the value
 * @return a function that converts the value of the source to the nearest {@code int}.
 */
template<typename Source>
auto int_value_of(const Source &source) -> std::function<int()> {
  return [&source]() -> float {
    return static_cast<int>(source.value);
  };
}

}
}
