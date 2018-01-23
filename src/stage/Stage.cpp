#include <cmath>
#include "Stage.hpp"
#include "util/Functions.hpp"
#include "util/DLatch.hpp"
#include "util/Ramp.hpp"

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
    const std::string Stage::SLUG = "Stage";
    const std::string Stage::NAME = Stage::SLUG;

    Stage::Stage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
              ramp(Ramp([this]() { return phaseIncrement(); })),
              stageInputFollower([this]() { return stageIn(); }),
              deferGate([this]() { return inputs[DEFER_GATE_IN].value; }),
              envelopeTrigger([this]() { return inputs[TRIGGER_IN].value; }) {
        deferGate.onRisingEdge([this]() { defer(); });
        deferGate.onFallingEdge([this]() { resume(); });

        envelopeTrigger.onRisingEdge([this]() { startEnvelope(); });

        ramp.onEndOfCycle([this]() { eocPulse.trigger(1e-3); });
    }

    void Stage::step() {
        deferGate.step();
        ramp.step();
        envelopeTrigger.step();

        outputs[STAGE_OUT].value = deferGate.isHigh() ? stageInputFollower.value() : envelopeOut();
        outputs[EOC_TRIGGER_OUT].value = toUnipolarVoltage(eocPulse.process(rack::engineGetSampleTime()));
        outputs[ACTIVE_GATE_OUT].value = toUnipolarVoltage(deferGate.isHigh() || ramp.isRunning());
    }

    float Stage::stageIn() const { return inputs[STAGE_IN].value; }

    float Stage::phaseIncrement() const {
        float knob = params[DURATION_KNOB].value;
        std::function<float(float)> squeezed(scalingToRange(DURATION_SQUEEZED_MIN, DURATION_SQUEEZED_MAX));
        std::function<float(float)> curved([](float f) { return pow(f, DURATION_CURVATURE); });
        std::function<float(float)> scaled(scalingToRange(0.0f, DURATION_SCALE));
        std::function<float(float)> phaseIncrementFor([](float f) { return rack::engineGetSampleTime() / f; });
        return phaseIncrementFor(scaled(curved(squeezed(knob))));
    }

    float Stage::level() const {
        return toUnipolarVoltage(params[LEVEL_KNOB].value);
    }

    float Stage::shape() const {
        std::function<float(float)> toShape(scalingToRange(-ENVELOPE_CURVATURE_MAX, ENVELOPE_CURVATURE_MAX));
        return toShape(params[SHAPE_KNOB].value);
    }

    void Stage::defer() {
        envelopeTrigger.pause();
        ramp.stop();
        stageInputFollower.follow();
    }

    void Stage::resume() {
        stageInputFollower.pause();
        envelopeTrigger.resume();
    }

    void Stage::startEnvelope() {
        stageInputFollower.pause();
        ramp.start();
    }

    float Stage::envelopeOut() {
        std::function<float(float)> scaled(scalingToRange(stageInputFollower.value(), level()));
        std::function<float(float)> shaped([this](float f){ return curved(f, shape()); });
        return scaled(shaped(ramp.phase()));
    }
} // namespace DHE
