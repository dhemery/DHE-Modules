#include "modules/sequencizer/control-ids.h"
#include "modules/sequencizer/signals.h"

#include "signals/durations.h"
#include "signals/gain.h"

#include "dheunit/test.h"
#include "helpers/assertions.h"
#include "helpers/rack-controls.h"

#include <functional>
#include <vector>

namespace test {
namespace sequencizer {
static auto constexpr N = 8; // NOLINT

using dhe::Duration;
using dhe::DurationRangeId;
using dhe::Gain;
using dhe::LongDuration;
using dhe::MediumDuration;
using dhe::ShortDuration;
using dhe::sequencizer::InputId;
using ParamId = dhe::sequencizer::ParamIds<N>;
using dhe::sequencizer::OutputId;
using LightId = dhe::sequencizer::LightIds<N>;
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::fake::Light;
using test::fake::Param;
using test::fake::Port;

struct DurationTestCase {
  std::string name;          // NOLINT
  float rotation;            // NOLINT
  DurationRangeId range;     // NOLINT
  float multiplier_rotation; // NOLINT
  float multiplier_cv;       // NOLINT
  float want;                // NOLINT
  float tolerance;           // NOLINT

  void run(Tester &t) const {
    t.run(name, [this](Tester &t) {
      std::vector<Param> params{ParamId::Count};
      std::vector<Port> inputs{InputId::Count};
      std::vector<Port> outputs{OutputId::Count};
      std::vector<Light> lights{LightId::Count};

      auto const step = std::rand() % N;
      auto const signals =
          dhe::sequencizer::Signals<Param, Port, Port, Light, N>{
              inputs, params, outputs, lights};
      params[ParamId::StepDuration + step].setValue(rotation);
      params[ParamId::DurationRange].setValue(static_cast<float>(range));
      params[ParamId::DurationMultiplier].setValue(multiplier_rotation);
      inputs[InputId::DurationMultiplierCV].setVoltage(multiplier_cv);

      auto const duration = signals.duration(step);
      assert_that(t, duration, is_near(want, tolerance));
    });
  }
};

static std::vector<DurationTestCase> test_cases = {
    {
        .name = "short range min rotation → 1ms",
        .rotation = 0.F,
        .range = DurationRangeId::Short,
        .multiplier_rotation = 0.5F, // 1x
        .multiplier_cv = 0.F,        /// No modulation from CV
        .want = 1e-3F,
    },
    {
        .name = "short range center rotation → 100ms",
        .rotation = 0.5F,
        .range = DurationRangeId::Short,
        .multiplier_rotation = 0.5F, // 1x
        .multiplier_cv = 0.F,        /// No modulation from CV
        .want = 1e-1F,
        .tolerance = 0.00001F,
    },
    {
        .name = "short range max rotation → 1s",
        .rotation = 1.F,
        .range = DurationRangeId::Short,
        .multiplier_rotation = 0.5F, // 1x
        .multiplier_cv = 0.F,        /// No modulation from CV
        .want = 1.F,
    },
    {
        .name = "short range 2x → 2x duration",
        .rotation = 0.781F,
        .range = DurationRangeId::Short,
        .multiplier_rotation = 1.F, // 2x
        .multiplier_cv = 0.F,       // No modulation from CV
        .want = ShortDuration::scale(0.781F) * 2.F,
    },
    {
        .name = "short range +5V cv increases multiplier by 1x",
        .rotation = 0.5F,
        .range = DurationRangeId::Short,
        .multiplier_rotation = 0.75F, // 1.5x
        .multiplier_cv = 5.F,         // 5V adds 1x
        .want = ShortDuration::scale(0.5F) * 2.5F,
    },
    {
        .name = "short 2x -5V cv reduces multiplier by 1x",
        .rotation = 0.5F,
        .range = DurationRangeId::Short,
        .multiplier_rotation = 0.75F, // 1.5x
        .multiplier_cv = -5.F,        // -5V subtracts 1x
        .want = ShortDuration::scale(0.5F) * 0.5F,
    },
    {
        .name = "limits minimum duration even with extreme negative CV",
        .rotation = 1.F, // Maximum nominal duration = 1s
        .range = DurationRangeId::Short,
        .multiplier_rotation = 0.F, // 0x
        .multiplier_cv = -3000.F,   // -3000V subtracts 600x
        .want = 1e-3F,              // Even with -600x
    },
};

/**
t.run("minimum medium duration is 1ms",
      test(0.F, DurationRangeId::Medium, 0.F, -30.F, is_equal_to(1e-3F)));

t.run("minimum long duration is 1ms",
      test(0.F, DurationRangeId::Long, 0.F, -30.F, is_equal_to(1e-3F)));

t.run("maximum short duration is 2s",
      test(1.F, DurationRangeId::Short, 1.F, 30.F, is_equal_to(2.F)));

t.run("maximum medium duration is 20s",
      test(1.F, DurationRangeId::Medium, 1.F, 30.F, is_equal_to(20.F)));

t.run("maximum long duration is 200s",
      test(1.F, DurationRangeId::Long, 1.F, 30.F, is_equal_to(200.F)));
*/

struct DurationSuite : Suite {
  DurationSuite() : Suite("dhe::sequencizer::Signals::duration()") {}

  void run(Tester &t) override {
    for (auto const &test : test_cases) {
      test.run(t);
    }
  }
};

static auto _ = DurationSuite{};
} // namespace sequencizer
} // namespace test
