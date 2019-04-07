#pragma once

#include <display/panel.h>
#include <modules/Tapers.h>

namespace DHE {

class TapersPanel : public Panel<TapersPanel> {
public:
  explicit TapersPanel(Tapers *module);
  static constexpr auto module_slug = "tapers";
  static constexpr auto hp = 9;
};
} // namespace DHE
