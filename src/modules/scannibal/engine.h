#pragma once
#include "components/phase-timer.h"
#include "components/range.h"

namespace dhe {
namespace scannibal {

template <typename Module, typename Anchor> class Engine {
public:
  Engine(Module &module, Anchor &start_anchor, Anchor &end_anchor)
      : module_{module}, start_anchor_{start_anchor}, end_anchor_{end_anchor} {}

  void start() { module_.show_status(0, 0.F); }

  void execute() {}

private:
  Module &module_;
  Anchor &start_anchor_;
  Anchor &end_anchor_;
  int step_{0};
};
} // namespace scannibal
} // namespace dhe
