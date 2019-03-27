#pragma once
#include <functional>
#include <utility>

#include "components/phase-accumulator.h"

namespace DHE {

/**
 * Advances its phase over the duration specified by the module, and generates an event phase ends.
 */
template <typename M>
class HoldGenerator : public PhaseAccumulator {
public:
  explicit HoldGenerator(M *module, std::function<void()> on_hold_complete)
      : module{module},
        on_hold_complete{std::move(on_hold_complete)} {}

  auto duration() const -> float override { return module->duration(); }

  auto sample_time() const -> float override { return module->sample_time(); }

  void on_finish() const override { on_hold_complete(); }

private:
  M *const module;
  const std::function<void()> on_hold_complete;
};
}
