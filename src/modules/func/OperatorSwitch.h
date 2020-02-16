#pragma once

#include "widgets/ControlWidgets.h"

#include <event.hpp>
#include <functional>
#include <utility>

namespace dhe {
namespace func {
  class OperatorSwitch : public Toggle {
  public:
    OperatorSwitch(std::function<void(bool)> onOperatorChange, std::string const &moduleSvgDir,
                   rack::engine::Module *module, float x, float y, int index) :
        Toggle{2, moduleSvgDir, module, x, y, index}, operatorChangedTo{std::move(onOperatorChange)} {}

    void onChange(const rack::event::Change &e) override {
      Toggle::onChange(e);
      auto isMultiply = this->paramQuantity->getValue() > 0.5;
      operatorChangedTo(isMultiply);
    }

  private:
    std::function<void(bool)> operatorChangedTo = [](bool /*unused*/) {};
  };
} // namespace func

} // namespace dhe
