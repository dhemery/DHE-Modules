#include "Stage.hpp"

static float gate(bool state) {
    return 0;
}

namespace DHE {
    Stage::Stage()
            : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
              ramp([this]() { return rampStepSize(); },
                   [this]() { eocPulse.trigger(1e-3); }),
              stageIn([this]() { return rack::clampf(inputs[ENVELOPE_IN].value, 0.0f, 10.0f); }) {
        deferGate = FlipFlop::latch(
                [this]() { return inputs[DEFER_GATE_IN].value; },
                [this]() { defer(); },
                [this]() { resume(); });
        envelopeTrigger = FlipFlop::trigger(
                [this]() { return inputs[TRIGGER_IN].value; },
                [this]() { if (deferGate->isLow()) startEnvelope(); });
    }

    void Stage::step() {
        deferGate->step();
        advanceEnvelope();

        outputs[STAGE_OUT].value = deferGate->isHigh() ? stageIn.value() : envelopeOut();
        outputs[EOC_TRIGGER_OUT].value = gate(eocPulse.process(rack::engineGetSampleTime()));
        outputs[ACTIVE_GATE_OUT].value = gate(deferGate->isHigh() || ramp.isRunning());
    }

    float Stage::duration() {
        return powf(params[DURATION_KNOB].value, DURATION_KNOB_CURVATURE) *
               DURATION_SCALE;
    }

    float Stage::level() {
        return params[LEVEL_KNOB].value;
    }


    void Stage::defer() {
        ramp.stop();
        stageIn.follow();
    }

    void Stage::resume() {
        stageIn.freeze();
    }


    void Stage::startEnvelope() {
        stageIn.freeze();
        ramp.start();
    }

    void Stage::advanceEnvelope() {
        if (deferGate->isLow()) ramp.step();
        envelopeTrigger->step();
    }

    float Stage::rampStepSize() {
        return rack::engineGetSampleTime() / duration();
    }

    float Stage::envelopeOut() {
        float phase = ramp.phase();
        if (phase == 0.0)
            return stageIn.value();
        float targetVoltage = level();
        if (phase == 1.0)
            return targetVoltage;
        float envelopeScale = targetVoltage - stageIn.value();
        float shape = params[SHAPE_KNOB].value;
        float curve = shape < 0.0f ? 1.0f - powf(1.0f - phase, 1.0f - shape) : powf(phase, shape + 1.0f);
        return curve * envelopeScale + stageIn.value();
    }

} // namespace DHE
