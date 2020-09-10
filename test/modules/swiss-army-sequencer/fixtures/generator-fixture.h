#include "modules/cv-sequencer/generator.h"

#include "components/range.h"
#include "components/sigmoid.h"

#include <array>
#include <dheunit/test.h>

namespace test {
namespace cv_sequencer {

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
  auto duration(int step) const -> float { return duration_[step]; }
  void output(float v) { output_ = v; }
  auto taper(int step) const -> dhe::sigmoid::Taper const & {
    return *taper_[step];
  }
  void show_progress(int step, float progress) { progress_[step] = progress; }
  void show_inactive(int step) { inactive_step_ = step; }

  std::array<float, step_count> curvature_{};                   // NOLINT
  std::array<float, step_count> progress_{};                    // NOLINT
  std::array<float, step_count> duration_{};                    // NOLINT
  int inactive_step_{};                                         // NOLINT
  float output_{};                                              // NOLINT
  std::array<dhe::sigmoid::Taper const *, step_count> taper_{}; // NOLINT
};

using Generator = dhe::cv_sequencer::Generator<Module, Anchor>;

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Anchor start_anchor{};
    Anchor end_anchor{};
    Generator generator{module, start_anchor, end_anchor};
    run(t, module, start_anchor, end_anchor, generator);
  };
}

} // namespace cv_sequencer
} // namespace test
