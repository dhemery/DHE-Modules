#pragma once

#include <string>
#include <vector>

namespace dhe {

struct Normal {
  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const names =
        std::vector<std::string>{"button-released", "button-pressed"};
    return names;
  }
};

struct Reversed {
  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const names = std::vector<std::string>{"output-button-released",
                                                       "output-button-pressed"};
    return names;
  }
};

struct Momentary {
  static auto constexpr momentary = true;
};

struct Toggle {
  static auto constexpr momentary = false;
};

} // namespace dhe
