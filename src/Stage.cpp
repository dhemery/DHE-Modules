#include "DHEModules.hpp"
#include "dsp/digital.hpp"

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

#define MAX_DURATION 10.0

struct Stage : Module {
	enum ParamIds {
		TIME_PARAM,
    SUSTAIN_PARAM,
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

  SchmittTrigger trigger;
  bool running = false;
  bool passingThru = false;
  float envelope = 0.0;
  int remainingSteps = 0;
  float out = 0.0;
  float delta = 0.0;
};

void Stage::step() {
  float sustain = params[SUSTAIN_PARAM].value;
  float envelopeIn = inputs[E_IN].value;
  passingThru = inputs[G_IN].value > 1.0;

  if(!running && trigger.process(inputs[T_IN].value)) {
    running = true;
    remainingSteps = params[TIME_PARAM].value * engineGetSampleRate();
    envelope = envelopeIn;
    delta = (sustain - envelope) / (float) remainingSteps;
  }

  if(remainingSteps > 0) {
    remainingSteps--;
    envelope += delta;
  } else {
    envelope = sustain;
    running = false;
  }

  out = passingThru ? envelopeIn : envelope;
  bool active = passingThru || running;

  outputs[E_OUT].value = out;
  outputs[T_OUT].value = !active ? 5.0 : -5.0;
  outputs[G_OUT].value = active ? 5.0 : -5.0;
}

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

  addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_TOP), module, Stage::TIME_PARAM, 0, 10.0, 0.0));
  addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_MIDDLE), module, Stage::SUSTAIN_PARAM, -5.0, 5.0, 0.0));
  addParam(createParam<RoundBlackKnob>(Vec(H_KNOB, V_KNOB_BOTTOM), module, Stage::CURVE_PARAM, -3.0, 3.0, 0.0));


  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_TOP), module, Stage::E_IN));
  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_MIDDLE), module, Stage::T_IN));
  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_BOTTOM), module,Stage::G_IN));

  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_TOP), module, Stage::E_OUT));
  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_MIDDLE), module, Stage::T_OUT));
  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_BOTTOM), module, Stage::G_OUT));
}
