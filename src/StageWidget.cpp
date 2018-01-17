#include "DHEModules.hpp"
#include "Stage.hpp"

#define ROGAN_P3_KNOB_SIZE 34.3
#define ROUND_BLACK_KNOB_SIZE 38.0
#define WIDGET_HP 6
#define WIDGET_WIDTH (WIDGET_HP * RACK_GRID_WIDTH)

#define KNOB_SIZE ROUND_BLACK_KNOB_SIZE
#define H_KNOB (WIDGET_WIDTH / 2.0 - KNOB_SIZE / 2.0)
#define V_KNOB_SPACING 55.0
#define V_KNOB_TOP 57.0
#define V_KNOB_MIDDLE (V_KNOB_TOP + V_KNOB_SPACING)
#define V_KNOB_BOTTOM (V_KNOB_MIDDLE + V_KNOB_SPACING)

#define V_PORT_SPACING 43.0
#define V_PORT_TOP 234
#define V_PORT_MIDDLE (V_PORT_TOP + V_PORT_SPACING)
#define V_PORT_BOTTOM (V_PORT_MIDDLE + V_PORT_SPACING)

#define PORT_WIDTH 23.0
#define H_PORT_INSET 25.0
#define H_PORT_LEFT (H_PORT_INSET - PORT_WIDTH / 2.0)
#define H_PORT_RIGHT (WIDGET_WIDTH - H_PORT_INSET - PORT_WIDTH / 2.0)

StageWidget::StageWidget() {
    Stage *module = new Stage();
    setModule(module);
    box.size = Vec(WIDGET_WIDTH, RACK_GRID_HEIGHT);

    SVGPanel *panel = new SVGPanel();
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

    addParam(createParam<RoundBlackKnob>(
        Vec(H_KNOB, V_KNOB_TOP), module, Stage::DURATION_KNOB,
        DURATION_KNOB_MIN, DURATION_KNOB_MAX, 0.5));
    addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_MIDDLE), module,
                                         Stage::LEVEL_KNOB, 0.0, 10.0, 0.0));
    addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_BOTTOM), module,
                                         Stage::SHAPE_KNOB, -(SHAPE_KNOB_MAX),
                                         SHAPE_KNOB_MAX, 0.0));

    addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_TOP), module,
                                     Stage::ENVELOPE_IN));
    addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_MIDDLE), module,
                                     Stage::TRIGGER_IN));
    addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_BOTTOM), module,
                                     Stage::DEFER_GATE_IN));

    addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_TOP), module,
                                       Stage::STAGE_OUT));
    addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_MIDDLE), module,
                                       Stage::EOC_TRIGGER_OUT));
    addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_BOTTOM), module,
                                       Stage::ACTIVE_GATE_OUT));
}
