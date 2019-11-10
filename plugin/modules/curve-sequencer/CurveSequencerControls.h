#pragma once

#include <common.hpp>

namespace dhe {

namespace curve_sequencer {

  template <int N> struct CurveSequencerControls {
    enum ParameterIds {
      RunButton,
      GateButton,
      SelectionStartKnob,
      SelectionLengthKnob,
      LoopButton,
      ResetButton,
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

    enum InputIds {
      CurveSequencerInput,
      GateInput,
      LoopInput,
      ResetInput,
      RunInput,
      ENUMS(EnabledInputs, N),
      InputCount
    };

    enum OutputIds { CurveSequencerOutput, OutputCount };

    enum LightIds { ENUMS(GeneratingLights, N), ENUMS(SustainingLights, N), LightCount };
  };

} // namespace curve_sequencer

} // namespace dhe
