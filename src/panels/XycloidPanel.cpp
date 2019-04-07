#include <panels/XycloidPanel.h>

#include <util/rotation.h>

namespace DHE {

XycloidPanel::XycloidPanel(Xycloid *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = widget_right_edge / 7.f;
  auto column_4 = widget_right_edge - column_1;
  auto column_2 = (column_4 - column_1) / 3.f + column_1;
  auto column_3 = widget_right_edge - column_2;

  auto y = 30.f;
  auto dy = 22.f;

  install(column_1, y, input(Xycloid::WOBBLE_RATIO_CV));
  install(column_2, y, knob<TinyKnob>(Xycloid::WOBBLE_RATIO_AV));
  install(column_3, y, knob<LargeKnob>(Xycloid::WOBBLE_RATIO_KNOB));
  install(column_4, y, toggle<2>(Xycloid::WOBBLE_RATIO_FREEDOM_SWITCH, 1));

  y += dy;
  install(column_1, y, input(Xycloid::WOBBLE_DEPTH_CV));
  install(column_2, y, knob<TinyKnob>(Xycloid::WOBBLE_DEPTH_AV));
  install(column_3, y, knob<LargeKnob>(Xycloid::WOBBLE_DEPTH_KNOB));
  install(column_4, y, toggle<3>(Xycloid::WOBBLE_RANGE_SWITCH, 2));

  y += dy;
  install(column_1, y, input(Xycloid::THROB_SPEED_CV));
  install(column_2, y, knob<TinyKnob>(Xycloid::THROB_SPEED_AV));
  install(column_3, y, knob<LargeKnob>(Xycloid::THROB_SPEED_KNOB, 0.65f));
  install(column_4, y, knob<SmallKnob>(Xycloid::WOBBLE_PHASE_KNOB));

  y = 82.f;
  dy = 15.f;
  const auto output_port_offset = 1.25f;

  auto default_gain = Rotation::gain_range.normalize(1.f);

  y += dy;
  install(column_1, y, input(Xycloid::X_GAIN_CV));
  install(column_2, y, knob<SmallKnob>(Xycloid::X_GAIN_KNOB, default_gain));
  install(column_3, y, toggle<2>(Xycloid::X_RANGE_SWITCH, 0));
  install(column_4, y + output_port_offset, output(Xycloid::X_OUT));

  y += dy;
  install(column_1, y, input(Xycloid::Y_GAIN_CV));
  install(column_2, y, knob<SmallKnob>(Xycloid::Y_GAIN_KNOB, default_gain));
  install(column_3, y, toggle<2>(Xycloid::Y_RANGE_SWITCH, 0));
  install(column_4, y + output_port_offset, output(Xycloid::Y_OUT));
}

void XycloidPanel::appendContextMenu(rack::Menu *menu) {
  auto xycloid = dynamic_cast<Xycloid *>(module);
  assert(xycloid);

  menu->addChild(rack::construct<rack::MenuLabel>());
  menu->addChild(
      rack::construct<rack::MenuLabel>(&rack::MenuLabel::text, "Options"));
  menu->addChild(new BooleanOption(
      "Musical Ratios",
      [xycloid](bool setting) {
        xycloid->set_musical_wobble_ratios(setting);
      },
      [xycloid] { return xycloid->is_musical_wobble_ratios(); }));
}

} // namespace DHE
