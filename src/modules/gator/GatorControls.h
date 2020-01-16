#pragma once

namespace dhe {
namespace gator {
  class GatorControls {
  public:
    static auto constexpr inputCount = 16;

    enum ParameterIds { ENUMS(NegateButtons, inputCount), ParameterCount };

    enum InputIds { ENUMS(Inputs, inputCount), InputCount };

    enum OutputIds {
      AndOutput,
      NandOutput,
      OrOutput,
      NorOutput,
      EvenOutput,
      OddOutput,
      XorOutput,
      XnorOutput,
      OutputCount
    };
  };
} // namespace gator
} // namespace dhe
