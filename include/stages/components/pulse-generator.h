

#pragma once
#include <functional>
#include <utility>

#include "components/phase-accumulator.h"

namespace DHE {

/**
 * Generates a pulse of the given length and generates events when the pulse
 * rises and falls.
 */
template <typename M> class PulseGenerator : public PhaseAccumulator {
public:
  explicit PulseGenerator(M *module, std::function<void()> on_eoc_rise,
                          std::function<void()> on_eoc_fall)
      : module{module}, on_eoc_rise{std::move(on_eoc_rise)},
        on_eoc_fall{std::move(on_eoc_fall)} {}

  void on_start() const override { on_eoc_rise(); }

  auto duration() const -> float override { return 1e-3; }

  auto sample_time() const -> float override { return module->sample_time(); }

  void on_finish() const override { on_eoc_fall(); }

private:
  M *const module;
  const std::function<void()> on_eoc_rise;
  const std::function<void()> on_eoc_fall;
};
} // namespace DHE
