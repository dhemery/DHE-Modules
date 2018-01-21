#include "DHEModules.hpp"
#include "Stage.hpp"

#define ROUND_BLACK_KNOB_SIZE 38.0f
#define WIDGET_HP 6
#define WIDGET_WIDTH (WIDGET_HP * RACK_GRID_WIDTH)

#define KNOB_SIZE ROUND_BLACK_KNOB_SIZE
#define H_KNOB (WIDGET_WIDTH / 2.0f - KNOB_SIZE / 2.0f)
#define V_KNOB_SPACING 55.0f
#define V_KNOB_TOP 57.0f
#define V_KNOB_MIDDLE (V_KNOB_TOP + V_KNOB_SPACING)
#define V_KNOB_BOTTOM (V_KNOB_MIDDLE + V_KNOB_SPACING)

#define V_PORT_SPACING 43.0f
#define V_PORT_TOP 234.0f
#define V_PORT_MIDDLE (V_PORT_TOP + V_PORT_SPACING)
#define V_PORT_BOTTOM (V_PORT_MIDDLE + V_PORT_SPACING)

#define PORT_WIDTH 23.0f
#define H_PORT_INSET 25.0f
#define H_PORT_LEFT (H_PORT_INSET - PORT_WIDTH / 2.0f)
#define H_PORT_RIGHT (WIDGET_WIDTH - H_PORT_INSET - PORT_WIDTH / 2.0f)

using namespace DHE;
using namespace rack;

StageWidget::StageWidget() {
    auto *module = new Stage();
    setModule(module);
    box.size = Vec(WIDGET_WIDTH, RACK_GRID_HEIGHT);

    auto *panel = new SVGPanel();
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/Stage.svg")));
    addChild(panel);

    Vec screwPositions[] = {
            Vec(RACK_GRID_WIDTH, 0), Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0),
            Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH),
            Vec(box.size.x - 2 * RACK_GRID_WIDTH,
                RACK_GRID_HEIGHT - RACK_GRID_WIDTH)};

    int screwCount = sizeof(screwPositions) / sizeof(*screwPositions);
    int blackScrew = rand() % screwCount;

    for (int i = 0; i < screwCount; i++) {
        if (i == blackScrew)
            addChild(createScrew<ScrewBlack>(screwPositions[i]));
        else
            addChild(createScrew<ScrewSilver>(screwPositions[i]));
    }

    addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_TOP), module, Stage::DURATION_KNOB, 0.0f, 1.0f, 0.5f));
    addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_MIDDLE), module, Stage::LEVEL_KNOB, 0.0, 10.0, 5.0));
    addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_BOTTOM), module, Stage::SHAPE_KNOB, 0.0f, 1.0f, 0.5f));

    addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_TOP), module, Stage::STAGE_IN));
    addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_MIDDLE), module, Stage::TRIGGER_IN));
    addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_BOTTOM), module, Stage::DEFER_GATE_IN));

    addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_TOP), module, Stage::STAGE_OUT));
    addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_MIDDLE), module, Stage::EOC_TRIGGER_OUT));
    addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_BOTTOM), module, Stage::ACTIVE_GATE_OUT));
}
