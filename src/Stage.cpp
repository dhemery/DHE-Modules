#include "Stage.hpp"

namespace DHE {
    Stage::Stage()
            : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
              ramp([this]() { return rampStepSize(); },
                   [this]() { eocPulse.trigger(1e-3); }),
              inPort([this]() { return rack::clampf(inputs[ENVELOPE_IN].value, 0.0f, 10.0f); }) {
        deferGate = FlipFlop::latch(
                [this]() { return inputs[DEFER_GATE_IN].value; },
                [this]() { defer(); },
                [this]() { resume(); });
        envelopeTrigger = FlipFlop::trigger(
                [this]() { return inputs[TRIGGER_IN].value; },
                [this]() { if(deferGate->isLow()) startEnvelope(); });
        activeGateOut = SwitchedOutput::gate(outputs[ACTIVE_GATE_OUT]);
        endOfCycleOut = SwitchedOutput::pulse(outputs[EOC_TRIGGER_OUT]);
        stageOut = SwitchedOutput::choosing(outputs[STAGE_OUT], [this]() { return inPort.value(); }, [this]() { return envelopeVoltage(); });
    }

    void Stage::step() {
        deferGate->step();
        advanceEnvelope();

        stageOut->send(deferGate->isHigh());
        endOfCycleOut->send(eocPulse.process(rack::engineGetSampleTime()));
        activeGateOut->send(deferGate->isHigh() || ramp.isRunning());
    }

    void Stage::startEnvelope() {
        inPort.freeze();
        ramp.start();
    }

    void Stage::defer() {
        ramp.stop();
        inPort.follow();
    }

    void Stage::resume() {
        inPort.freeze();
    }

    void Stage::advanceEnvelope() {
        if (deferGate->isLow()) ramp.step();
        envelopeTrigger->step();
    }

    float Stage::rampStepSize() {
        float duration =
                powf(params[DURATION_KNOB].value, DURATION_KNOB_CURVATURE) *
                DURATION_SCALE;
        return rack::engineGetSampleTime() / duration;
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
