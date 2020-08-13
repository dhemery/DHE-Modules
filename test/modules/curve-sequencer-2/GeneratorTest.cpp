#include "modules/curve-sequencer-2/Generator.h"

#include <doctest.h>
#include <string>

namespace test {
namespace curve_sequencer_2 {
  namespace sustainer {

    struct GeneratorControls {
    public:
      std::function<void(int, float)> showProgress{
          [](int s, float p) { throw "showProgress " + std::to_string(s) + " " + std::to_string(p); }};
      std::function<void(int)> showInactive{[](int s) { throw "showInactive " + std::to_string(s); }};
    };
    struct GeneratorSource {};

    using Generator = dhe::curve_sequencer_2::Generator<GeneratorControls, GeneratorSource>;

    static inline void start(Generator &generator, GeneratorControls &controls, int step) {
      controls.showProgress = [](int /**/, float /**/) {};
      generator.start(step);
      controls.showProgress
          = [](int s, float p) { throw "showProgress " + std::to_string(s) + " " + std::to_string(p); };
    }

    TEST_CASE("curve_sequencer_2::Generator") {
      GeneratorControls controls{};
      GeneratorSource startSource{};
      GeneratorSource endSource{};

      Generator generator{controls, startSource, endSource};

      SUBCASE("start") {
        SUBCASE("shows step progress is 0") {
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
      }

      SUBCASE("stop") {
        SUBCASE("deactivates the current step") {
          auto constexpr step{7};
          start(generator, controls, step);

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
