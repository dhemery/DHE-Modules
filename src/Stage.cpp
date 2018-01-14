#include "DHEModules.hpp"

#define V_PORT_SPACING 40
#define V_PORT_TOP 235
#define V_PORT_MIDDLE (V_PORT_TOP + V_PORT_SPACING)
#define V_PORT_BOTTOM (V_PORT_MIDDLE + V_PORT_SPACING)

#define H_PORT_SPACING 38
#define H_PORT_LEFT 13
#define H_PORT_RIGHT (H_PORT_LEFT + H_PORT_SPACING)

struct Stage : Module {
	enum ParamIds {
		TIME_PARAM,
    CURVE_PARAM,
    END_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
    IN_INPUT,
    OVERRIDE_INPUT,
    TRIGGER_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
    OUT_OUTPUT,
    EOC_OUTPUT,
    ACTIVE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float phase = 0.0;
	float blinkPhase = 0.0;

	Stage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};


void Stage::step() {
  outputs[OUT_OUTPUT].value = params[TIME_PARAM].value;
  outputs[EOC_OUTPUT].value = params[CURVE_PARAM].value;
  outputs[ACTIVE_OUTPUT].value = params[END_PARAM].value;
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

  addParam(createParam<RoundBlackKnob>(Vec(28, 57), module, Stage::TIME_PARAM, 0, 10.0, 0.0));
  addParam(createParam<RoundBlackKnob>(Vec(28, 97), module, Stage::CURVE_PARAM, -3.0, 3.0, 0.0));
  addParam(createParam<RoundBlackKnob>(Vec(28, 137), module, Stage::END_PARAM, -5.0, 5.0, 0.0));

  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_TOP), module, Stage::OVERRIDE_INPUT));
  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_MIDDLE), module, Stage::TRIGGER_INPUT));
  addInput(createInput<PJ301MPort>(Vec(H_PORT_LEFT, V_PORT_BOTTOM), module, Stage::IN_INPUT));

  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_TOP), module, Stage::ACTIVE_OUTPUT));
  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_MIDDLE), module, Stage::EOC_OUTPUT));
  addOutput(createOutput<PJ301MPort>(Vec(H_PORT_RIGHT, V_PORT_BOTTOM), module, Stage::OUT_OUTPUT));
}
