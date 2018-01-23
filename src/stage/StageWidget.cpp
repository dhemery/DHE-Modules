#include "Stage.hpp"
#include "StageWidget.hpp"

namespace DHE {
    using namespace rack;
    StageWidget::StageWidget() : Widget(new Stage(), 4, "res/Stage Skinny.svg") {
        createScrews(SCREW_H_INSET, SCREW_V_INSET);

        addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_TOP), module, Stage::DURATION_KNOB, 0.0f,
                                                   1.0f, 0.5f));
        addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_MIDDLE), module, Stage::LEVEL_KNOB, 0.0,
                                                   1.0f, 0.5f));
        addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_BOTTOM), module, Stage::SHAPE_KNOB, 0.0f,
                                                   1.0f, 0.5f));

        addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_TOP), module, Stage::STAGE_IN));
        addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_MIDDLE), module, Stage::TRIGGER_IN));
        addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_BOTTOM), module, Stage::DEFER_GATE_IN));

        addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_TOP), module, Stage::STAGE_OUT));
        addOutput(
                createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_MIDDLE), module, Stage::EOC_TRIGGER_OUT));
        addOutput(
                createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_BOTTOM), module, Stage::ACTIVE_GATE_OUT));
    }

}