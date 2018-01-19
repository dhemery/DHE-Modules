#include "Stage.hpp"

#define TRIGGER_HI (10.0f)
#define TRIGGER_LO (0.0f)
#define GATE_HI (5.0f)
#define GATE_LO (-5.0f)


namespace DHE {
    Stage::Stage()
            : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
              ramp([&]() { return rampStepSize(); },
                   [&]() { eocPulse.trigger(1e-3); }),
              inPort([&]() { return rack::clampf(inputs[ENVELOPE_IN].value, 0.0f, 10.0f); }) {
    }

    void Stage::step() {
        deferLatch.process(inputs[DEFER_GATE_IN].value,
                           [&]() {
                               ramp.stop();
                               inPort.follow();
                           },
                           [&]() {
                               inPort.freeze();
                           });

        if (deferLatch.isLow()) {
            advanceEnvelope();
        }

        outputs[STAGE_OUT].value = stageOutVoltage();
        outputs[EOC_TRIGGER_OUT].value = eocTriggerOutVoltage();
        outputs[ACTIVE_GATE_OUT].value = activeGateOutVoltage();
    }

    void Stage::advanceEnvelope() {
        if (trigger.process(inputs[TRIGGER_IN].value) == Latch::RISE) {
            inPort.freeze();
            ramp.start();
        } else {
            ramp.step();
        }
    }

    float Stage::activeGateOutVoltage() {
        return (deferLatch.isHigh() || ramp.isRunning()) ? GATE_HI : GATE_LO;
    }

    float Stage::eocTriggerOutVoltage() {
        return eocPulse.process(1.0f / rack::engineGetSampleRate()) ? TRIGGER_HI
                                                                    : TRIGGER_LO;
    }

    float Stage::stageOutVoltage() {
        return deferLatch.isHigh() ? inPort.value() : envelopeVoltage();
    }

    float Stage::rampStepSize() {
        float duration =
                powf(params[DURATION_KNOB].value, DURATION_KNOB_CURVATURE) *
                DURATION_SCALE;
        return 1.0f / (duration * rack::engineGetSampleRate());
    }

    float Stage::envelopeVoltage() {
        float phase = ramp.phase();
        if (phase == 0.0)
            return inPort.value();
        float targetVoltage = params[LEVEL_KNOB].value;
        if (phase == 1.0)
            return targetVoltage;
        float envelopeScale = targetVoltage - inPort.value();
        float shape = params[SHAPE_KNOB].value;
        float curve = shape < 0.0f ? 1.0f - powf(1.0f - phase, 1.0f - shape) : powf(phase, shape + 1.0f);
        return curve * envelopeScale + inPort.value();
    }

} // namespace DHE
