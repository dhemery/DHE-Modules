#pragma once
#include <type_traits>

template <typename E>
static constexpr auto enum_index(E e) ->
    typename std::underlying_type<E>::type {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

namespace dhe {
template <typename E, E First, E Last> class EnumIterator {
  using ValueT = typename std::underlying_type<E>::type;

public:
  EnumIterator() : EnumIterator{First} {}
  explicit EnumIterator(E const &f) : value_(enum_index(f)) {}

  auto begin() -> EnumIterator { return *this; }
  auto end() -> EnumIterator {
    static const EnumIterator<E, First, Last> end_iter =
        ++EnumIterator<E, First, Last>(Last);
    return end_iter;
  }

  auto operator*() -> E { return static_cast<E>(value_); }
  auto operator++() -> EnumIterator {
    ++value_;
    return *this;
  }
  auto operator!=(const EnumIterator &i) const -> bool {
    return value_ != i.value_;
  }
  auto value() const -> ValueT { return value_; }

private:
  explicit EnumIterator(ValueT value) : value_{value} {}
  ValueT value_;
};

} // namespace dhe
