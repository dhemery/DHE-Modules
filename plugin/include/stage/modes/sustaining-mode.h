#pragma once

#include "stage-mode.h"

namespace DHE {

template <typename M> class SustainMode : public StageMode<M> {
public:
  explicit SustainMode(M *module)
      : module{module} {}

  void step() override { module->do_sustain(); }

private:
  M *module;
};
} // namespace DHE
