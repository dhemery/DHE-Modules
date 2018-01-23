#pragma once

#include "gui/Widget.hpp"

namespace DHE {
struct StageWidget : public Widget {

    // Per widget
    constexpr static const float WIDGET_HP = 4;
    constexpr static const float WIDGET_WIDTH = WIDGET_HP * RACK_GRID_WIDTH;
    constexpr static const float SCREW_H_INSET = 7.5f;
    constexpr static const float SCREW_V_INSET = 0.0f;

    constexpr static const float KNOB_SIZE = Widget::ROUND_BLACK_KNOB_DIAMETER;
    constexpr static const float H_KNOB = WIDGET_WIDTH / 2.0f - KNOB_SIZE / 2.0f;

    constexpr static const float V_KNOB_SPACING = 55.0f;
    constexpr static const float V_KNOB_TOP = 57.0f;
    constexpr static const float V_KNOB_MIDDLE = V_KNOB_TOP + V_KNOB_SPACING;
    constexpr static const float V_KNOB_BOTTOM = V_KNOB_MIDDLE + V_KNOB_SPACING;
    constexpr static const float V_PORT_SPACING = 43.0f;
    constexpr static const float V_PORT_TOP = 234.0f;
    constexpr static const float V_PORT_MIDDLE = V_PORT_TOP + V_PORT_SPACING;
    constexpr static const float V_PORT_BOTTOM = V_PORT_MIDDLE + V_PORT_SPACING;

    constexpr static const float H_PORT_INSET = RACK_GRID_WIDTH;
    constexpr static const float H_PORT_LEFT = H_PORT_INSET - Widget::PORT_DIAMETER / 2.0f;
    constexpr static const float H_PORT_RIGHT = WIDGET_WIDTH - H_PORT_INSET - Widget::PORT_DIAMETER / 2.0f;

    StageWidget();
};
}