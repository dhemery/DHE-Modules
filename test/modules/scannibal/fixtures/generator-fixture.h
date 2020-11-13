#include "modules/scannibal/generator.h"

#include "components/range.h"
#include "components/sigmoid.h"

#include <array>
#include <dheunit/test.h>

namespace test {
namespace scannibal {

using dhe::unit::Tester;
using dhe::unit::TestFunc;

auto constexpr step_count = 8;
struct Anchor {
  void enter(int step) { entered_[step] = true; }
  auto voltage() const -> float { return voltage_; }

  std::array<bool, step_count> entered_{}; // NOLINT
  float voltage_{};                        // NOLINT
};

struct Module {
  auto curvature(int step) const -> float { return curvature_[step]; }
  void output(float v) { output_ = v; }
  auto taper(int step) const -> dhe::sigmoid::Taper const & {
    return *taper_[step];
  }

  std::array<float, step_count> curvature_{};                   // NOLINT
  float output_{};                                              // NOLINT
  std::array<dhe::sigmoid::Taper const *, step_count> taper_{}; // NOLINT
};

using Generator = dhe::scannibal::Generator<Module, Anchor>;

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Anchor phase_0_anchor{};
    Anchor phase_1_anchor{};
    Generator generator{module, phase_0_anchor, phase_1_anchor};
    run(t, module, phase_0_anchor, phase_1_anchor, generator);
  };
}

} // namespace scannibal
} // namespace test
