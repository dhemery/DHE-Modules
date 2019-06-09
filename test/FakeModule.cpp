#include <engine/Module.hpp>

rack::engine::Module::Module() {}
rack::engine::Module::~Module() {}

void rack::engine::Module::config(int numParams, int numInputs, int numOutputs, int numLights) {
  params = std::vector<Param>(numParams);
  inputs = std::vector<Input>(numInputs);
  outputs = std::vector<Output>(numOutputs);
  lights = std::vector<Light>(numLights);
  paramQuantities = std::vector<ParamQuantity *>(numParams);
}
