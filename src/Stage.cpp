#include "DHEModules.hpp"
#include "dsp/digital.hpp"
#include "util.hpp"

// TODO: Switch for slow, medium, and fast ramp
// These constants yield ramp durations of:
//    knob fully ccw  : .002417s
//    knob dead center: 1s
//    knob fully cw   : 10s
#define DURATION_KNOB_CURVATURE (4.0)
#define DURATION_KNOB_MAX (0.88913970)
#define DURATION_KNOB_MIN (1.0 - DURATION_KNOB_MAX)
#define DURATION_SCALE (16.0)

#define SHAPE_KNOB_MAX 4.0

#define EOC_PULSE_LENGTH (1e-3)

struct Ramp {
    float value = 1.0;

    void start() { value = 0.0; }

    void step(float duration) {
        float delta = 0.5 / (duration * engineGetSampleRate());
        value = clampf(value + delta, 0.0, 1.0);
    }

    bool running() { return value < 1.0; }
};

struct Stage : Module {
    enum ParamIds { DURATION_KNOB, LEVEL_KNOB, SHAPE_KNOB, NUM_PARAMS };
    enum InputIds { ENVELOPE_IN, TRIGGER_IN, DEFER_GATE_IN, NUM_INPUTS };
    enum OutputIds {
        ENVELOPE_OUT,
        EOC_TRIGGER_OUT,
        ACTIVE_GATE_OUT,
        NUM_OUTPUTS
    };
    enum LightIds { NUM_LIGHTS };

    Stage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
        trigger.setThresholds(0.0, 1.0);
    }
    void step() override;
    float envelopeValue();

    Ramp ramp;
    SchmittTrigger trigger;
    PulseGenerator eocPulse;
    float envelopeOffset;
};

void Stage::step() {
    float envelopeIn = clampf(inputs[ENVELOPE_IN].value, 0.0, 10.0);
    float passingThru = inputs[DEFER_GATE_IN].value > 1.0;

    if (ramp.running()) {
        float duration =
            powf(params[DURATION_KNOB].value, DURATION_KNOB_CURVATURE) *
            DURATION_SCALE;
        ramp.step(duration);
        if (!ramp.running())
            eocPulse.trigger(EOC_PULSE_LENGTH);
    } else if (trigger.process(inputs[TRIGGER_IN].value)) {
        envelopeOffset = envelopeIn;
        ramp.start();
    }

    float out = passingThru ? envelopeIn : envelopeValue();
    bool active = passingThru || ramp.running();
    bool sendEndOfCyclePulse = eocPulse.process(1.0 / engineGetSampleRate());

    outputs[ENVELOPE_OUT].value = out;
    outputs[EOC_TRIGGER_OUT].value = sendEndOfCyclePulse ? 10.0 : 0;
    outputs[ACTIVE_GATE_OUT].value = active ? 5.0 : -5.0;
}

float Stage::envelopeValue() {
    float envelopeScale = params[LEVEL_KNOB].value - envelopeOffset;
    float curve = params[SHAPE_KNOB].value;
    float curvature = curve < 0.0 ? -1.0 / (curve - 1.0) : curve + 1.0;
    return powf(ramp.value, curvature) * envelopeScale + envelopeOffset;
}

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

    addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_TOP), module,
                                         Stage::DURATION_KNOB, DURATION_KNOB_MIN,
                                         DURATION_KNOB_MAX, 0.5));
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
                                       Stage::ENVELOPE_OUT));
    addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_MIDDLE), module,
                                       Stage::EOC_TRIGGER_OUT));
    addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_BOTTOM), module,
                                       Stage::ACTIVE_GATE_OUT));
}
