#include "modules/curve-sequencer/CurveSequencerControls.h"

#include "components/Taper.h"
#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"
#include "doctest/doctest.h"
#include "modules/curve-sequencer/GenerateMode.h"

namespace test {
namespace curve_sequencer_controls {

TEST_CASE("curve_sequencer::CurveSequencerControls") {
  static auto constexpr stepCount{8};

  using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;
  using dhe::curve_sequencer::AdvanceMode;
  using dhe::curve_sequencer::GenerateMode;

  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Output> outputs{Controls::OutputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  std::vector<rack::engine::Light> lights{Controls::LightCount};

  Controls controls{inputs, outputs, params, lights};

  SUBCASE("showInactive(step) dims step progress lights") {
    auto constexpr step = 3;
    auto constexpr completedProgressLightIndex = step + step;
    auto constexpr remainingProgressLightIndex = step + step + 1;

    controls.showInactive(step);

    CHECK_EQ(lights[Controls::ProgressLights + completedProgressLightIndex]
                 .getBrightness(),
             0.F);
    CHECK_EQ(lights[Controls::ProgressLights + remainingProgressLightIndex]
                 .getBrightness(),
             0.F);
  }
}
} // namespace curve_sequencer_controls
} // namespace test
