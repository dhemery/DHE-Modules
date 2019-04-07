#pragma once

#include <display/panel.h>
#include <modules/Func.h>

namespace DHE {
class FuncPanel : public Panel<FuncPanel> {
public:
  explicit FuncPanel(Func *func);
  static constexpr auto module_slug = "func";
  static constexpr auto hp = 3;
};
} // namespace DHE
