#pragma once
#include <engine/Module.hpp>

namespace dhe {

class Gator : public rack::engine::Module {

public:
  Gator();
  void process(const ProcessArgs &args) override;

  static auto constexpr inputCount = 16;

  enum ParameterIds { NegateButtons, ParameterCount = NegateButtons + inputCount };

  enum InputIds { Inputs, InputCount = Inputs + inputCount };

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

} // namespace dhe
