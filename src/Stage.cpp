#include "Stage.hpp"

#define TRIGGER_HI (10.0)
#define TRIGGER_LO (0.0)
#define GATE_HI (5.0)
#define GATE_LO (-5.0)

namespace DHE {

Stage::Stage()
    : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
      ramp([&]() { eocPulse.trigger(1e-3); }) {}

void Stage::step() {
    float inputVoltage = rack::clampf(inputs[ENVELOPE_IN].value, 0.0, 10.0);

    deferLatch.process(inputs[DEFER_GATE_IN].value, [&]() { ramp.stop(); },
                       [&]() { hold(inputVoltage); });

    if (deferLatch.isLow()) {
        advanceEnvelopeIfRunning();
        startEnvelopeIfTriggered(inputVoltage);
    }

    outputs[STAGE_OUT].value = stageOutVoltage(inputVoltage);
    outputs[EOC_TRIGGER_OUT].value = eocTriggerOutVoltage();
    outputs[ACTIVE_GATE_OUT].value = activeGateOutVoltage();
}

float Stage::activeGateOutVoltage() {
    return deferLatch.isHigh() || ramp.running ? GATE_HI : GATE_LO;
}

float Stage::eocTriggerOutVoltage() {
    return eocPulse.process(1.0 / rack::engineGetSampleRate()) ? TRIGGER_HI
                                                               : TRIGGER_LO;
}

void Stage::hold(float newHoldVoltage) { holdVoltage = newHoldVoltage; }

float Stage::stageOutVoltage(float deferredVoltage) {
    return deferLatch.isHigh() ? deferredVoltage : envelopeVoltage();
}

void Stage::advanceEnvelopeIfRunning() {
    if (ramp.running)
        advanceEnvelope();
}

void Stage::advanceEnvelope() {
    float duration =
        powf(params[DURATION_KNOB].value, DURATION_KNOB_CURVATURE) *
        DURATION_SCALE;
    ramp.step(duration);
}

float Stage::envelopeVoltage() {
    float envelopeScale = params[LEVEL_KNOB].value - holdVoltage;
    float shape = params[SHAPE_KNOB].value;
    float curvature = shape < 0.0 ? -1.0 / (shape - 1.0) : shape + 1.0;
    return powf(ramp.value, curvature) * envelopeScale + holdVoltage;
}

void Stage::startEnvelopeIfTriggered(float startVoltage) {
    trigger.process(inputs[TRIGGER_IN].value, [&]() {
        hold(startVoltage);
        ramp.start();
    });
}
} // namespace DHE
