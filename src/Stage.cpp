#include "Stage.hpp"
#include "rack.hpp"

#define TRIGGER_HI (10.0)
#define TRIGGER_LO (0.0)
#define GATE_HI (5.0)
#define GATE_LO (-5.0)

Stage::Stage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
    trigger.setThresholds(0.0, 1.0);
}

void Stage::step() {
    float inputVoltage = clampf(inputs[ENVELOPE_IN].value, 0.0, 10.0);
    float wasDeferring = deferring;
    deferring = inputs[DEFER_GATE_IN].value > 1.0;
    if(wasDeferring && !deferring) {
        // DEFER gate went low after previous step
        startVoltage = inputVoltage;
    }

    if (deferring) {
        ramp.stop();
    } else {
        if (ramp.running) {
            ramp.step(duration());
            if (!ramp.running) {
                // Ramp completed during this step, so start the EOC pulse.
                eocPulse.trigger(EOC_PULSE_LENGTH);
            }
        }
        if (trigger.process(inputs[TRIGGER_IN].value)) {
            startVoltage = inputVoltage;
            ramp.start();
        }
    }

    bool active = deferring || ramp.running;
    bool pulsingEoC = eocPulse.process(1.0 / engineGetSampleRate());

    outputs[ENVELOPE_OUT].value = deferring ? inputVoltage : envelopeVoltage();
    outputs[EOC_TRIGGER_OUT].value = pulsingEoC ? TRIGGER_HI : TRIGGER_LO;
    outputs[ACTIVE_GATE_OUT].value = active ? GATE_HI : GATE_LO;
}

float Stage::envelopeVoltage() {
    float envelopeScale = params[LEVEL_KNOB].value - startVoltage;
    float shape = params[SHAPE_KNOB].value;
    float curvature = shape < 0.0 ? -1.0 / (shape - 1.0) : shape + 1.0;
    return powf(ramp.value, curvature) * envelopeScale + startVoltage;
}

float Stage::duration() {
    return powf(params[DURATION_KNOB].value, DURATION_KNOB_CURVATURE) *
           DURATION_SCALE;
}
