#include "DHEModules.hpp"
#include "dsp/digital.hpp"
#include "util.hpp"

// TODO: Switch for slow, medium, and fast ramp speed
// These constants yield ramp durations of:
//    knob fully ccw  : .002417s
//    knob dead center: 1s
//    knob fully cw   : 10s
#define DURATION_CURVE_EXPONENT 4.0
#define DURATION_SCALE 16.0
#define RATE_KNOB_MAX 0.88913970
#define RATE_KNOB_MIN (1.0-RATE_KNOB_MAX)

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
    E_IN,
    T_IN,
    G_IN,
		NUM_INPUTS
	};
	enum OutputIds {
    E_OUT,
    T_OUT,
    G_OUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Stage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
    trigger.setThresholds(0.0, 1.0);
  }
	void step() override;

  Ramp ramp;
  SchmittTrigger trigger;
  float envelopeOffset;
  float envelopeScale;
};

void Stage::step() {
  float envelopeIn = inputs[E_IN].value;
  float passingThru = inputs[G_IN].value > 1.0;

  if(ramp.running()) {
    float duration = powf(1.0 - params[RATE_PARAM].value, DURATION_CURVE_EXPONENT) * DURATION_SCALE;
    ramp.step(duration);
  } else if (trigger.process(inputs[T_IN].value)) {
    envelopeOffset = envelopeIn;
    ramp.start();
  }

  float envelopeScale = params[LEVEL_PARAM].value - envelopeOffset;
  float out = passingThru ? envelopeIn : ramp.value * envelopeScale + envelopeOffset;
  bool active = passingThru || ramp.running();

  outputs[E_OUT].value = out;
  outputs[T_OUT].value = !active ? 5.0 : -5.0;
  outputs[G_OUT].value = active ? 5.0 : -5.0;
}

#define H_KNOB 27
#define V_KNOB_SPACING 55
#define V_KNOB_TOP 57
#define V_KNOB_MIDDLE (V_KNOB_TOP + V_KNOB_SPACING)
#define V_KNOB_BOTTOM (V_KNOB_MIDDLE + V_KNOB_SPACING)

#define V_PORT_SPACING 43
#define V_PORT_TOP 234
#define V_PORT_MIDDLE (V_PORT_TOP + V_PORT_SPACING)
#define V_PORT_BOTTOM (V_PORT_MIDDLE + V_PORT_SPACING)

#define H_PORT_SPACING 38
#define H_PORT_LEFT 13
#define H_PORT_RIGHT (H_PORT_LEFT + H_PORT_SPACING)

StageWidget::StageWidget() {
	Stage *module = new Stage();
	setModule(module);
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/Stage.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

  addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_TOP), module, Stage::RATE_PARAM, RATE_KNOB_MIN, RATE_KNOB_MAX, 0.5));
  addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_MIDDLE), module, Stage::LEVEL_PARAM, -5.0, 5.0, 0.0));
  addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_BOTTOM), module, Stage::CURVE_PARAM, -3.0, 3.0, 0.0));


  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_TOP), module, Stage::E_IN));
  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_MIDDLE), module, Stage::T_IN));
  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_BOTTOM), module,Stage::G_IN));

  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_TOP), module, Stage::E_OUT));
  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_MIDDLE), module, Stage::T_OUT));
  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_BOTTOM), module, Stage::G_OUT));
}
