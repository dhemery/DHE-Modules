#pragma once

#include <type_traits>
template <typename E, E First, E Last> class Iterator {
  using ValueT = typename std::underlying_type<E>::type;

public:
  Iterator() : Iterator{First} {}
  explicit Iterator(E const &f) : value_(static_cast<ValueT>(f)) {}

  auto begin() -> Iterator { return *this; }
  auto end() -> Iterator {
    static const Iterator<E, First, Last> end_iter =
        ++Iterator<E, First, Last>(Last);
    return end_iter;
  }

  auto operator*() -> E { return static_cast<E>(value_); }
  auto operator++() -> Iterator {
    ++value_;
    return *this;
  }
  auto operator!=(const Iterator &i) const -> bool {
    return value_ != i.value_;
  }
  auto value() const -> ValueT { return value_; }

private:
  explicit Iterator(ValueT value) : value_{value} {}
  ValueT value_;
};
