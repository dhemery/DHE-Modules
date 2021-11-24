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

struct DurationRangeTest {
  std::string name;          // NOLINT
  float rotation;            // NOLINT
  float multiplier_rotation; // NOLINT
  float multiplier_cv;       // NOLINT
  float want;                // NOLINT
  float tolerance;           // NOLINT

  void run(Tester &t, DurationRangeId range_id) const {
    t.run(name, [this, range_id](Tester &t) {
      std::vector<Param> params{ParamId::Count};
      std::vector<Port> inputs{InputId::Count};
      std::vector<Port> outputs{OutputId::Count};
      std::vector<Light> lights{LightId::Count};

      auto const step = std::rand() % N;
      auto const signals =
          dhe::sequencizer::Signals<Param, Port, Port, Light, N>{
              inputs, params, outputs, lights};
      params[ParamId::StepDuration + step].setValue(rotation);
      params[ParamId::DurationRange].setValue(static_cast<float>(range_id));
      params[ParamId::DurationMultiplier].setValue(multiplier_rotation);
      inputs[InputId::DurationMultiplierCV].setVoltage(multiplier_cv);

      auto const duration = signals.duration(step);
      assert_that(t, duration, is_near(want, tolerance));
    });
  }
};

struct DurationRangeSuite {
  DurationRangeId range_id;             // NOLINT
  std::vector<DurationRangeTest> tests; // NOLINT

  void run(Tester &t) const {
    auto const name =
        std::string{"With range "} + dhe::internal::duration::label(range_id);
    t.run(name, [this](Tester &t) {
      for (auto const &test : tests) {
        test.run(t, range_id);
      }
    });
  }
};

static auto short_duration_tests = DurationRangeSuite{
    .range_id = DurationRangeId::Short,
    .tests =
        {
            {
                .name = "min rotation → 1ms",
                .rotation = 0.F,
                .multiplier_rotation = 0.5F, // 1x
                .multiplier_cv = 0.F,        /// No modulation from CV
                .want = 1e-3F,
            },
            {
                .name = "center rotation → 100ms",
                .rotation = 0.5F,
                .multiplier_rotation = 0.5F, // 1x
                .multiplier_cv = 0.F,        /// No modulation from CV
                .want = 1e-1F,
                .tolerance = 0.00001F,
            },
            {
                .name = "max rotation → 1s",
                .rotation = 1.F,
                .multiplier_rotation = 0.5F, // 1x
                .multiplier_cv = 0.F,        /// No modulation from CV
                .want = 1.F,
            },
            {
                .name = "max multiplier rotation → 2x duration",
                .rotation = 0.781F,
                .multiplier_rotation = 1.F, // 2x
                .multiplier_cv = 0.F,       // No modulation from CV
                .want = ShortDuration::scale(0.781F) * 2.F,
                .tolerance = 0.00001F,
            },
            {
                .name = "+5V cv increases multiplier by 1x",
                .rotation = 0.5F,
                .multiplier_rotation = 0.75F, // 1.5x
                .multiplier_cv = 5.F,         // 5V adds 1x
                .want = ShortDuration::scale(0.5F) * 2.5F,
            },
            {
                .name = "-5V cv reduces multiplier by 1x",
                .rotation = 0.5F,
                .multiplier_rotation = 0.75F, // 1.5x
                .multiplier_cv = -5.F,        // -5V subtracts 1x
                .want = ShortDuration::scale(0.5F) * 0.5F,
            },
            {
                .name = "limits minimum duration even with extreme negative CV",
                .rotation = 1.F,            // Maximum nominal duration = 1s
                .multiplier_rotation = 0.F, // 0x
                .multiplier_cv = -3000.F,   // -3000V subtracts 600x
                .want = 1e-3F,              // Even with -600x
            },
        },
};

static auto medium_duration_tests = DurationRangeSuite{
    .range_id = DurationRangeId::Medium,
    .tests =
        {
            {
                .name = "min rotation → 10ms",
                .rotation = 0.F,
                .multiplier_rotation = 0.5F, // 1x
                .multiplier_cv = 0.F,        /// No modulation from CV
                .want = 1e-2F,
            },
            {
                .name = "center rotation → 1s",
                .rotation = 0.5F,
                .multiplier_rotation = 0.5F, // 1x
                .multiplier_cv = 0.F,        /// No modulation from CV
                .want = 1.F,
                .tolerance = 0.0001F,
            },
            {
                .name = "max rotation → 10s",
                .rotation = 1.F,
                .multiplier_rotation = 0.5F, // 1x
                .multiplier_cv = 0.F,        /// No modulation from CV
                .want = 10.F,
            },
            {
                .name = "max multiplier rotation → 2x duration",
                .rotation = 0.781F,
                .multiplier_rotation = 1.F, // 2x
                .multiplier_cv = 0.F,       // No modulation from CV
                .want = MediumDuration::scale(0.781F) * 2.F,
                .tolerance = 0.00001F,
            },
            {
                .name = "+5V cv increases multiplier by 1x",
                .rotation = 0.5F,
                .multiplier_rotation = 0.75F, // 1.5x
                .multiplier_cv = 5.F,         // 5V adds 1x
                .want = MediumDuration::scale(0.5F) * 2.5F,
            },
            {
                .name = "-5V cv reduces multiplier by 1x",
                .rotation = 0.5F,
                .multiplier_rotation = 0.75F, // 1.5x
                .multiplier_cv = -5.F,        // -5V subtracts 1x
                .want = MediumDuration::scale(0.5F) * 0.5F,
            },
            {
                .name = "limits minimum duration even with extreme negative CV",
                .rotation = 1.F,            // Maximum nominal duration = 1s
                .multiplier_rotation = 0.F, // 0x
                .multiplier_cv = -3000.F,   // -3000V subtracts 600x
                .want = 1e-3F,              // Even with -600x
            },
        },
};

static auto long_duration_tests = DurationRangeSuite{
    .range_id = DurationRangeId::Long,
    .tests =
        {
            {
                .name = "min rotation → 10ms",
                .rotation = 0.F,
                .multiplier_rotation = 0.5F, // 1x
                .multiplier_cv = 0.F,        /// No modulation from CV
                .want = 1e-1F,
            },
            {
                .name = "center rotation → 10s",
                .rotation = 0.5F,
                .multiplier_rotation = 0.5F, // 1x
                .multiplier_cv = 0.F,        /// No modulation from CV
                .want = 10.F,
                .tolerance = 0.001F,
            },
            {
                .name = "max rotation → 100s",
                .rotation = 1.F,
                .multiplier_rotation = 0.5F, // 1x
                .multiplier_cv = 0.F,        /// No modulation from CV
                .want = 100.F,
            },
            {
                .name = "max multiplier rotation → 2x duration",
                .rotation = 0.781F,
                .multiplier_rotation = 1.F, // 2x
                .multiplier_cv = 0.F,       // No modulation from CV
                .want = LongDuration::scale(0.781F) * 2.F,
                .tolerance = 0.00001F,
            },
            {
                .name = "+5V cv increases multiplier by 1x",
                .rotation = 0.5F,
                .multiplier_rotation = 0.75F, // 1.5x
                .multiplier_cv = 5.F,         // 5V adds 1x
                .want = LongDuration::scale(0.5F) * 2.5F,
            },
            {
                .name = "-5V cv reduces multiplier by 1x",
                .rotation = 0.5F,
                .multiplier_rotation = 0.75F, // 1.5x
                .multiplier_cv = -5.F,        // -5V subtracts 1x
                .want = LongDuration::scale(0.5F) * 0.5F,
            },
            {
                .name = "limits minimum duration even with extreme negative CV",
                .rotation = 1.F,            // Maximum nominal duration = 1s
                .multiplier_rotation = 0.F, // 0x
                .multiplier_cv = -3000.F,   // -3000V subtracts 600x
                .want = 1e-3F,              // Even with -600x
            },
        },
};

struct DurationSuite : Suite {
  DurationSuite() : Suite("dhe::sequencizer::Signals::duration()") {}

  void run(Tester &t) override {
    short_duration_tests.run(t);
    medium_duration_tests.run(t);
    long_duration_tests.run(t);
  }
};

static auto _ = DurationSuite{};
} // namespace sequencizer
} // namespace test
