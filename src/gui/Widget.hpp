#pragma once

#include "app.hpp"
#include "engine.hpp"

namespace DHE {
class Widget : public rack::ModuleWidget {

public:
    constexpr static const float ROUND_BLACK_KNOB_DIAMETER = 38.0f;
    constexpr static const float PORT_DIAMETER = 23.0f;
    constexpr static const float SCREW_DIAMETER = 15.0f;

    Widget(rack::Module *module, int widgetWidth, const char *background);

    void createScrews(float horizontal_inset, float vertical_inset);
};

}
