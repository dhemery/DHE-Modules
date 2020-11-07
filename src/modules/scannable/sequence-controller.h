#pragma once

#include "components/latch.h"
#include "components/phase-timer.h"

namespace dhe {
namespace scannable {

template <typename Module, typename Generator> class SequenceController {
public:
  SequenceController(Module &module, Generator &generator)
      : module_{module}, generator_{generator} {}

  void execute(float phase) {}

  int step_{-1};
  Module &module_;
  Generator &generator_;
};
} // namespace scannable
} // namespace dhe
