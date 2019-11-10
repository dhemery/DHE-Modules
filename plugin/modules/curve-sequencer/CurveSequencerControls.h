#pragma once

#include <common.hpp>

namespace dhe {

namespace curve_sequencer {

  template <int N> struct CurveSequencerControls {
    enum ParameterIds {
      RunButton,
      ResetButton,
      GateButton,
      SelectionStartKnob,
      SelectionLengthKnob,
      DurationRangeSwitch,
      LevelRangeSwitch,
      ENUMS(CurveKnobs, N),
      ENUMS(DurationKnobs, N),
      ENUMS(EnabledButtons, N),
      ENUMS(LevelKnobs, N),
      ENUMS(GenerateModeSwitches, N),
      ENUMS(SustainModeSwitches, N),
      ENUMS(ShapeSwitches, N),
      ParameterCount
    };

    enum InputIds { GateInput, ResetInput, RunInput, StartCVInput, StepsCVInput, ENUMS(EnabledInputs, N), InputCount };

    enum OutputIds { OutOutput, OutputCount };

    enum LightIds { ENUMS(GeneratingLights, N), ENUMS(SustainingLights, N), LightCount };
  };

} // namespace curve_sequencer

} // namespace dhe
