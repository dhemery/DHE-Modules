#pragma once
#include "components/phase-timer.h"
#include "components/range.h"

namespace dhe {
namespace scannibal {

template <typename Module, typename Generator> class Controller {
public:
  Controller(Module &module, Generator &generator)
      : module_{module}, generator_{generator} {}

  void execute() {}

private:
  Module &module_;
  Generator &generator_;
  int step_{0};
};
} // namespace scannibal
} // namespace dhe
