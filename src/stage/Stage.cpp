#include <cmath>
#include "Stage.hpp"
#include "engine.hpp"

// These constants yield ramp durations of:
//    knob fully ccw  : .002417s
//    knob dead center: 1s
//    knob fully cw   : 10s
#define DURATION_CURVATURE (4.0f)
#define DURATION_SCALE (16.0f)
#define DURATION_SQUEEZED_MAX (0.88913970f)
#define DURATION_SQUEEZED_MIN (1.0f - DURATION_SQUEEZED_MAX)
#define ENVELOPE_CURVATURE_MAX 4.0f

inline float curved(float phase, float shape) {
    return shape >= 0.0f ? pow(phase, shape + 1.0f) : 1.0f - curved(1.0f - phase, -shape);
}

namespace DHE {
const char *Stage::SLUG = "Stage";
const char *Stage::NAME = Stage::SLUG;

Stage::Stage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
                 deferGate{[this]() { return inputs[DEFER_GATE_IN].value; }},
                 endOfCyclePulse{1000.0, &rack::engineGetSampleTime},
                 envelopeRamp{[this]() { return rate(); }, &rack::engineGetSampleTime},
                 envelopeTrigger{[this]() { return inputs[TRIGGER_IN].value; }},
                 stageInputFollower{[this]() { return stageIn(); }} {
    deferGate.onRisingEdge([this]() { defer(); });
    deferGate.onFallingEdge([this]() { resume(); });

    envelopeTrigger.onRisingEdge([this]() { startEnvelope(); });

    envelopeRamp.onEndOfCycle([this]() { endOfCyclePulse.start(); });
}

void Stage::step() {
    deferGate.step();
    envelopeRamp.step();
    envelopeTrigger.step();
    endOfCyclePulse.step();

    outputs[STAGE_OUT].value = deferGate.isHigh() ? stageInputFollower.value() : envelopeOut();
    outputs[EOC_TRIGGER_OUT].value = toUnipolarVoltage(endOfCyclePulse.isActive());
    outputs[ACTIVE_GATE_OUT].value = toUnipolarVoltage(deferGate.isHigh() || envelopeRamp.isActive());
}

float Stage::stageIn() const { return inputs[STAGE_IN].value; }

float Stage::rate() const {
    float knob = params[DURATION_KNOB].value;
    std::function<float(float)> squeezed(scalingToRange(DURATION_SQUEEZED_MIN, DURATION_SQUEEZED_MAX));
    std::function<float(float)> curved([](float f) { return pow(f, DURATION_CURVATURE); });
    std::function<float(float)> scaled(scalingToRange(0.0f, DURATION_SCALE));
    return 1.0f / scaled(curved(squeezed(knob)));
}

float Stage::level() const {
    return toUnipolarVoltage(params[LEVEL_KNOB].value);
}

float Stage::shape() const {
    std::function<float(float)> toShape(scalingToRange(-ENVELOPE_CURVATURE_MAX, ENVELOPE_CURVATURE_MAX));
    return toShape(params[SHAPE_KNOB].value);
}

void Stage::defer() {
    envelopeTrigger.suspendFiring();
    envelopeRamp.stop();
    stageInputFollower.follow();
}

void Stage::resume() {
    stageInputFollower.pause();
    envelopeTrigger.resumeFiring();
}

void Stage::startEnvelope() {
    stageInputFollower.pause();
    envelopeRamp.start();
}

float Stage::envelopeOut() {
    std::function<float(float)> scaled(scalingToRange(stageInputFollower.value(), level()));
    std::function<float(float)> shaped([this](float f) { return curved(f, shape()); });
    return scaled(shaped(envelopeRamp.phase()));
}
} // namespace DHE
