#include "Stage.hpp"
#include "StageWidget.hpp"

namespace DHE {
static constexpr int WIDGET_HP = 4;
static constexpr int WIDGET_WIDTH = WIDGET_HP*RACK_GRID_WIDTH;
static constexpr float WIDGET_CENTER = WIDGET_WIDTH/2.0f;
static constexpr float SCREW_H_INSET = 7.5f;
static constexpr float SCREW_V_INSET = 0.0f;

static constexpr float KNOB_RADIUS = Widget::ROUND_BLACK_KNOB_DIAMETER/2.0f;
static constexpr float H_KNOB = WIDGET_CENTER - KNOB_RADIUS;

static constexpr float V_KNOB_SPACING = 55.0f;
static constexpr float V_KNOB_TOP = 57.0f;
static constexpr float V_KNOB_MIDDLE = V_KNOB_TOP + V_KNOB_SPACING;
static constexpr float V_KNOB_BOTTOM = V_KNOB_MIDDLE + V_KNOB_SPACING;
static constexpr float V_PORT_SPACING = 43.0f;
static constexpr float V_PORT_TOP = 234.0f;
static constexpr float V_PORT_MIDDLE = V_PORT_TOP + V_PORT_SPACING;
static constexpr float V_PORT_BOTTOM = V_PORT_MIDDLE + V_PORT_SPACING;

static constexpr float H_PORT_INSET = RACK_GRID_WIDTH;
static constexpr float H_PORT_LEFT = H_PORT_INSET - Widget::PORT_RADIUS;
static constexpr float H_PORT_RIGHT = WIDGET_WIDTH - H_PORT_INSET - Widget::PORT_RADIUS - 2.0f;

StageWidget::StageWidget() : Widget(new Stage(), WIDGET_WIDTH, "res/Stage Skinny.svg") {
  createScrews(SCREW_H_INSET, SCREW_V_INSET);

  installParam<rack::RoundBlackKnob>(Stage::DURATION_KNOB, H_KNOB, V_KNOB_TOP);
  installParam<rack::RoundBlackKnob>(Stage::LEVEL_KNOB, H_KNOB, V_KNOB_MIDDLE);
  installParam<rack::RoundBlackKnob>(Stage::SHAPE_KNOB, H_KNOB, V_KNOB_BOTTOM);

  installInput<rack::PJ301MPort>(Stage::STAGE_IN, H_PORT_LEFT, V_PORT_TOP);
  installInput<rack::PJ301MPort>(Stage::TRIGGER_IN, H_PORT_LEFT, V_PORT_MIDDLE);
  installInput<rack::PJ301MPort>(Stage::DEFER_GATE_IN, H_PORT_LEFT, V_PORT_BOTTOM);

  installOutput<rack::PJ301MPort>(Stage::STAGE_OUT, H_PORT_RIGHT, V_PORT_TOP);
  installOutput<rack::PJ301MPort>(Stage::EOC_TRIGGER_OUT, H_PORT_RIGHT, V_PORT_MIDDLE);
  installOutput<rack::PJ301MPort>(Stage::ACTIVE_GATE_OUT, H_PORT_RIGHT, V_PORT_BOTTOM);
}
}