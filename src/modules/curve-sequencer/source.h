#pragma once

#include <ostream>

namespace dhe {
namespace curve_sequencer {
enum class Source { Level, In, Out };

static auto constexpr source_count = static_cast<int>(Source::Out) + 1;

static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::Source source)
    -> std::ostream & {
  os << static_cast<int>(source);
  return os;
}

template <typename ControlsT> class SourceController {
public:
  SourceController(ControlsT &controls) {}
  void snap(int /*step*/) {}
};
} // namespace curve_sequencer
} // namespace dhe
