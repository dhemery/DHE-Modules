#pragma once

namespace dhe {

namespace fuzzy_logic {

class Controls : public rack::engine::Module {
public:
  enum ParameterIds {
    ENUMS(NotAButtons, 2),
    ENUMS(NotBButtons, 2),
    LevelRangeSwitch,
    ParameterCount
  };

  enum InputIds { ENUMS(AInputs, 2), ENUMS(BInputs, 2), InputCount };

  enum OutputIds {
    ENUMS(AndOutputs, 2),
    ENUMS(NandOutputs, 2),
    ENUMS(OrOutputs, 2),
    ENUMS(NorOutputs, 2),
    ENUMS(XorOutputs, 2),
    ENUMS(XnorOutputs, 2),
    ENUMS(ImplicationOutputs, 2),
    ENUMS(NonimplicationOutputs, 2),
    ENUMS(ConverseImplicationOutputs, 2),
    ENUMS(ConverseNonimplicationOutputs, 2),
    OutputCount
  };
};
} // namespace fuzzy_logic
} // namespace dhe
