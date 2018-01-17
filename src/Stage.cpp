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
    bool wasDeferring = deferGate.isHigh();
    deferGate.process(inputs[DEFER_GATE_IN].value);

    if (deferGate.isHigh() && !wasDeferring) {
        ramp.stop();
    } else if (wasDeferring && !deferGate.isHigh()) {
        hold(inputVoltage);
    }

    if (!deferGate.isHigh()) {
        if (ramp.running)
            advanceEnvelope();
        if (triggered())
            startEnvelope(inputVoltage);
    }

    outputs[STAGE_OUT].value = stageOutVoltage(inputVoltage);
    outputs[EOC_TRIGGER_OUT].value = eocTriggerOutVoltage();
    outputs[ACTIVE_GATE_OUT].value = activeGateOutVoltage();
}

float Stage::activeGateOutVoltage() {
    return deferGate.isHigh() || ramp.running ? GATE_HI : GATE_LO;
}

void Stage::advanceEnvelope() {
    float duration =
        powf(params[DURATION_KNOB].value, DURATION_KNOB_CURVATURE) *
        DURATION_SCALE;
    ramp.step(duration, eocPulse);
}

float Stage::envelopeVoltage() {
    float envelopeScale = params[LEVEL_KNOB].value - holdVoltage;
    float shape = params[SHAPE_KNOB].value;
    float curvature = shape < 0.0 ? -1.0 / (shape - 1.0) : shape + 1.0;
    return powf(ramp.value, curvature) * envelopeScale + holdVoltage;
}

float Stage::eocTriggerOutVoltage() {
    return eocPulse.process(1.0 / engineGetSampleRate()) ? TRIGGER_HI
                                                         : TRIGGER_LO;
}

void Stage::hold(float newHoldVoltage) { holdVoltage = newHoldVoltage; }

float Stage::stageOutVoltage(float deferredVoltage) {
    return deferGate.isHigh() ? deferredVoltage : envelopeVoltage();
}

void Stage::startEnvelope(float startVoltage) {
    hold(startVoltage);
    ramp.start();
}
bool Stage::triggered() { return trigger.process(inputs[TRIGGER_IN].value); }
