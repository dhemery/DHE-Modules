#pragma once
namespace dhe {
namespace tapers {
  class TapersControls {
  public:
    enum ParameterIds {
      LevelKnob1,
      LevelAvKnob1,
      LevelRangeSwitch1,
      CurveKnob1,
      CurveAvKnob1,
      ShapeSwitch1,
      LevelKnob2,
      LevelAvKnob2,
      LevelRangeSwitch2,
      CurveKnob2,
      CurveAvKnob2,
      ShapeSwitch2,
      ParameterCount
    };
    enum InputIds { LevelCvInput1, CurveCvInput1, LevelCvInput2, CurveCvInput2, InputCount };
    enum OutputIds { TaperOutput1, TaperOutput2, OutputCount };
  };

} // namespace tapers
} // namespace dhe