#include "modules/curve-sequencer-2/Generator.h"

#include <doctest.h>
#include <functional>
#include <string>

namespace test {
namespace curve_sequencer_2 {
  namespace sustainer {

    struct GeneratorControls {
      std::function<void(int, float)> showProgress{
          [](int s, float p) { throw "showProgress " + std::to_string(s) + " " + std::to_string(p); }};
      std::function<void(int)> showInactive{[](int s) { throw "showInactive " + std::to_string(s); }};
    };

    struct GeneratorSource {
      std::function<void(int)> snap = [](int s) { throw "snap " + std::to_string(s); };
    };

    using Generator = dhe::curve_sequencer_2::Generator<GeneratorControls, GeneratorSource>;

    static inline void setCurrentStep(Generator &generator, GeneratorControls &controls, GeneratorSource &startSource,
                                      GeneratorSource &endSource, int step) {
      controls.showProgress = [](int /**/, float /**/) {};
      startSource.snap = [](int s) {};
      endSource.snap = [](int s) {};
      generator.start(step);
      controls.showProgress
          = [](int s, float p) { throw "showProgress " + std::to_string(s) + " " + std::to_string(p); };
      startSource.snap = [](int s) { throw "start.snap" + std::to_string(s); };
      endSource.snap = [](int s) { throw "end.snap" + std::to_string(s); };
    }

    TEST_CASE("curve_sequencer_2::Generator") {
      GeneratorControls controls{};
      GeneratorSource startSource{};
      GeneratorSource endSource{};

      Generator generator{controls, startSource, endSource};

      std::function<void(int)> givenCurrentStep
          = [&](int s) { setCurrentStep(generator, controls, startSource, endSource, s); };

      SUBCASE("start") {
        SUBCASE("shows step progress is 0") {
          startSource.snap = [](int s) {};
          endSource.snap = [](int s) {};
          auto constexpr step{3};

          auto activatedStep{-1};
          auto progress{-1.F};
          controls.showProgress = [&activatedStep, &progress](int s, float p) {
            activatedStep = s;
            progress = p;
          };

          generator.start(step);

          CHECK_EQ(activatedStep, step);
          CHECK_EQ(progress, 0.F);
        }

        SUBCASE("snaps sources with step 0") {
          controls.showProgress = [](int s, float p) {};
          auto constexpr step{0};

          auto snappedStartStep{-1};
          startSource.snap = [&snappedStartStep](int s) { snappedStartStep = s; };
          auto snappedEndStep{-1};
          endSource.snap = [&snappedEndStep](int s) { snappedEndStep = s; };

          generator.start(step);

          CHECK_EQ(snappedStartStep, step);
          CHECK_EQ(snappedEndStep, step);
        }
      }

      SUBCASE("stop") {
        SUBCASE("deactivates the current step") {
          auto constexpr step{7};
          givenCurrentStep(step);

          auto deactivatedStep{-1};
          controls.showInactive = [&deactivatedStep](int s) { deactivatedStep = s; };

          generator.stop();

          CHECK_EQ(deactivatedStep, step);
        }
      }
    }
  } // namespace sustainer
} // namespace curve_sequencer_2
} // namespace test
