#include "DHEModules.hpp"
#include "dsp/digital.hpp"
#include "util.hpp"

// TODO: Switch for slow, medium, and fast ramp speed
// These constants yield ramp durations of:
//    knob fully ccw  : .002417s
//    knob dead center: 1s
//    knob fully cw   : 10s
#define DURATION_CURVE_EXPONENT (4.0)
#define DURATION_SCALE (16.0)
#define RATE_KNOB_MAX (0.88913970)
#define RATE_KNOB_MIN (1.0-RATE_KNOB_MAX)

#define CURVE_MAX 4.0

#define END_OF_CYCLE_PULSE_LENGTH (1e-3)

struct Ramp {
  float value = 1.0;

  void start() {
    value = 0.0;
  }

  void step(float duration) {
    float delta = 0.5 / (duration * engineGetSampleRate());
    value = clampf(value + delta, 0.0, 1.0);
  }

  bool running() {
    return value < 1.0;
  }
};

struct Stage : Module {
	enum ParamIds {
		RATE_PARAM,
    LEVEL_PARAM,
    CURVE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
    ENVELOPE_IN,
    TRIGGER_IN,
    PASSTHROUGH_GATE_IN,
		NUM_INPUTS
	};
	enum OutputIds {
    ENVELOPE_OUT,
    END_OF_CYCLE_TRIGGER_OUT,
    ACTIVE_GATE_OUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

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
  float passingThru = inputs[PASSTHROUGH_GATE_IN].value > 1.0;

  if(ramp.running()) {
    float duration = powf(1.0 - params[RATE_PARAM].value, DURATION_CURVE_EXPONENT) * DURATION_SCALE;
    ramp.step(duration);
    if(!ramp.running()) eocPulse.trigger(END_OF_CYCLE_PULSE_LENGTH);
  } else if (trigger.process(inputs[TRIGGER_IN].value)) {
    envelopeOffset = envelopeIn;
    ramp.start();
  }

  float out = passingThru ? envelopeIn : envelopeValue();
  bool active = passingThru || ramp.running();
  bool sendEndOfCyclePulse =  eocPulse.process(1.0/engineGetSampleRate());

  outputs[ENVELOPE_OUT].value = out;
  outputs[END_OF_CYCLE_TRIGGER_OUT].value = sendEndOfCyclePulse ? 10.0 : 0;
  outputs[ACTIVE_GATE_OUT].value = active ? 5.0 : -5.0;
}

float Stage::envelopeValue() {
  float envelopeScale = params[LEVEL_PARAM].value - envelopeOffset;
  float curve = params[CURVE_PARAM].value;
  float curvature = curve > 0.0 ? 1.0 / (curve + 1) : 1 - curve;
  return powf(ramp.value, curvature) * envelopeScale + envelopeOffset;
}

#define ROGAN_P3_KNOB_SIZE 34.3
#define ROUND_BLACK_KNOB_SIZE 38.0
#define WIDGET_HP 6
#define WIDGET_WIDTH (WIDGET_HP * RACK_GRID_WIDTH)

#define KNOB_SIZE ROUND_BLACK_KNOB_SIZE
#define H_KNOB (WIDGET_WIDTH/2.0 - KNOB_SIZE/2.0)
#define V_KNOB_SPACING 55.0
#define V_KNOB_TOP 57.0
#define V_KNOB_MIDDLE (V_KNOB_TOP + V_KNOB_SPACING)
#define V_KNOB_BOTTOM (V_KNOB_MIDDLE + V_KNOB_SPACING)

#define V_PORT_SPACING 43.0
#define V_PORT_TOP 234
#define V_PORT_MIDDLE (V_PORT_TOP + V_PORT_SPACING)
#define V_PORT_BOTTOM (V_PORT_MIDDLE + V_PORT_SPACING)

#define PORT_WIDTH 23.0
#define H_PORTRIGGERSET 25.0
#define H_PORT_LEFT (H_PORTRIGGERSET-PORT_WIDTH/2.0)
#define H_PORT_RIGHT (WIDGET_WIDTH-H_PORTRIGGERSET-PORT_WIDTH/2.0)

StageWidget::StageWidget() {
	Stage *module = new Stage();
	setModule(module);
	box.size = Vec(WIDGET_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/Stage.svg")));
		addChild(panel);
	}

  Vec screwPositions[] = {
    Vec(RACK_GRID_WIDTH, 0),
    Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0),
    Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH),
    Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)
  };

  int screwCount = sizeof(screwPositions)/sizeof(*screwPositions);
  int blackScrew = rand() % screwCount;

  for(int i = 0 ; i < screwCount ; i++) {
    if(i == blackScrew) addChild(createScrew<ScrewBlack>(screwPositions[i]));
    else addChild(createScrew<ScrewSilver>(screwPositions[i]));
  }

  addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_TOP), module, Stage::RATE_PARAM, RATE_KNOB_MIN, RATE_KNOB_MAX, 0.5));
  addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_MIDDLE), module, Stage::LEVEL_PARAM, -0.0, 10.0, 0.0));
  addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_BOTTOM), module, Stage::CURVE_PARAM, -(CURVE_MAX), CURVE_MAX, 0.0));

  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_TOP), module, Stage::ENVELOPE_IN));
  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_MIDDLE), module, Stage::TRIGGER_IN));
  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_BOTTOM), module,Stage::PASSTHROUGH_GATE_IN));

  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_TOP), module, Stage::ENVELOPE_OUT));
  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_MIDDLE), module, Stage::END_OF_CYCLE_TRIGGER_OUT));
  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_BOTTOM), module, Stage::ACTIVE_GATE_OUT));
}
