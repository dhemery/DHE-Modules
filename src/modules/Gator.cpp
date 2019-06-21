#include "modules/Gator.h"

#include "modules/controls/CommonConfig.h"

#include <string>

dhe::Gator::Gator() {
  config(ParameterCount, InputCount, OutputCount);
  for (auto i = 0; i < inputCount; i++) {
    configParam(NegateButtons + i, 0.F, 1.F, 0.F, "Negate input " + std::to_string(i + 1));
  }
}

void dhe::Gator::process(const rack::engine::Module::ProcessArgs &args) {}
