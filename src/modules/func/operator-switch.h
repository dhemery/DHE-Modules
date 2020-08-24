#pragma once

#include "widgets/control-widgets.h"

#include <event.hpp>
#include <functional>
#include <utility>

namespace dhe {
namespace func {
class OperatorSwitch : public Toggle {
public:
  OperatorSwitch(std::function<void(bool)> on_operator_change,
                 std::string const &module_svg_dir,
                 rack::engine::Module *module, float x, float y, int index)
      : Toggle{2, module_svg_dir, module, x, y, index},
        operator_changed_to_{std::move(on_operator_change)} {}

  void onChange(const rack::event::Change &e) override {
    Toggle::onChange(e);
    auto const is_multiply = this->paramQuantity->getValue() > 0.5;
    operator_changed_to_(is_multiply);
  }

private:
  std::function<void(bool)> operator_changed_to_ = [](bool /*unused*/) {};
};
} // namespace func

} // namespace dhe
