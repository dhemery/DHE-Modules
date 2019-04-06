#include "engine.hpp"

#include "display/controls.h"
#include "display/panel.h"
#include <modules/BoosterStage.h>
#include <panels/BoosterStagePanel.h>

namespace DHE {

BoosterStagePanel::BoosterStagePanel(BoosterStage *module) : Panel{module, hp} {
auto widget_right_edge = width();

auto column_1 = widget_right_edge / 6.f + 0.3333333f;
auto column_3 = widget_right_edge / 2.f;
auto column_5 = widget_right_edge - column_1;
auto button_port_distance = 7.891f;
auto column_2 = column_1 + button_port_distance;
auto column_4 = column_5 - button_port_distance;

auto y = 25.f;
auto dy = 18.5f;

install(column_1, y, input(BoosterStage::LEVEL_CV));
install(column_3, y, knob<LargeKnob>(BoosterStage::LEVEL_KNOB));
install(column_5, y,
    toggle<2>(BoosterStage::LEVEL_RANGE_SWITCH, 1,
              module->level_range_selector));

y += dy;
install(column_1, y, input(BoosterStage::CURVE_CV));
install(column_3, y, knob<LargeKnob>(BoosterStage::CURVE_KNOB));
install(column_5, y, toggle<2>(BoosterStage::SHAPE_SWITCH, 0));

y += dy;
install(column_1, y, input(BoosterStage::DURATION_CV));
install(column_3, y, knob<LargeKnob>(BoosterStage::DURATION_KNOB));
install(column_5, y,
    toggle<3>(BoosterStage::DURATION_RANGE_SWITCH, 1,
              module->duration_range_selector));

y = 82.f;
dy = 15.f;

install(column_1, y, input(BoosterStage::DEFER_GATE_IN));
install(column_2, y, button(BoosterStage::DEFER_BUTTON));
install(column_4, y,
    button<ReverseButton>(
    BoosterStage::ACTIVE_BUTTON,
    [module](bool active) { module->set_active_button(active); }));
install(column_5, y, output(BoosterStage::ACTIVE_OUT));

y += dy;
install(column_1, y, input(BoosterStage::STAGE_TRIGGER_IN));
install(column_2, y, button(BoosterStage::TRIGGER_BUTTON));
install(column_4, y,
    button<ReverseButton>(BoosterStage::EOC_BUTTON, [module](bool eoc) {
  module->set_eoc_button(eoc);
}));
install(column_5, y, output(BoosterStage::EOC_OUT));

y += dy;
install(column_1, y, input(BoosterStage::ENVELOPE_IN));
install(column_5, y, output(BoosterStage::MAIN_OUT));
}
}
