#include "modules/scannibal/generator.h"

#include "components/range.h"
#include "signals/curvature.h"

#include "dheunit/test.h"

#include <array>
#include <functional>

namespace test {
namespace scannibal {

using dhe::sigmoid::ShapeId;
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

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
  auto shape(int step) const -> ShapeId { return shape_[step]; }

  std::array<float, step_count> curvature_{}; // NOLINT
  float output_{};                            // NOLINT
  std::array<ShapeId, step_count> shape_{};   // NOLINT
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
