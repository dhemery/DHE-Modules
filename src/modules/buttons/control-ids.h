#pragma once

#define MULTIPLE(name, n) name, name##_LAST = (name) + (n)-1

namespace dhe {
namespace buttons {

static auto constexpr button_count = 8;

struct ParamIds {
  enum {
    MULTIPLE(Button, button_count),
    MULTIPLE(Negate, button_count),
    Count,
  };
};

struct OutputIds {
  enum {
    MULTIPLE(Out, button_count),
    Count,
  };
};

} // namespace buttons
} // namespace dhe

#undef MULTIPLE
