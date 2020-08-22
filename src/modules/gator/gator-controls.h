#pragma once

namespace dhe {
namespace gator {
class GatorControls {
public:
  static auto constexpr input_count = 16;

  enum ParameterIds { ENUMS(NegateButtons, input_count), ParameterCount };

  enum InputIds { ENUMS(Inputs, input_count), InputCount };

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
