#pragma once
#include <utility>

#include <app/SvgSwitch.hpp>
#include <componentlibrary.hpp>

namespace DHE {

template <typename P, int N> class Toggles : public rack::app::SvgSwitch {
public:
  explicit Toggles(const std::string &name = "toggle-" + std::to_string(N)) {
    auto base = name + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(P::svg(base + std::to_string(position)));
    }
  }

  static constexpr auto size = N;
};

/**
 * A toggle that announces each new position.
 * @tparam P the type of panel to which the toggle is attached
 * @tparam N the number of toggle positions
 */
template <typename P, int N> class AnnouncingToggle : public Toggles<P, N> {
public:
  void onChange(const rack::event::Change &e) override {
    auto const value = this->paramQuantity->getValue();
    auto const position = static_cast<int>(value);
    announce(position);
    Toggles<P, N>::onChange(e);
  }

  std::function<void(int switchPosition)> announce;
};
} // namespace DHE
