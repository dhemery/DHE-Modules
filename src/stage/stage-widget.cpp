#include "stage.h"
#include "stage-widget.h"

namespace DHE {

StageWidget::StageWidget() : Widget(new Stage(), 4, "res/Stage.svg") {
  float widgetRightEdge = this->box.size.x;

  float topKnobY = 75.0f;
  float knobSpacing = 55.0f;
  float knobX = widgetRightEdge / 2.0f;
  installParam<rack::RoundBlackKnob>(Stage::DURATION_KNOB, knobX, topKnobY);
  installParam<rack::RoundBlackKnob>(Stage::LEVEL_KNOB, knobX, topKnobY + knobSpacing);
  installParam<rack::RoundBlackKnob>(Stage::SHAPE_KNOB, knobX, topKnobY + 2 * knobSpacing);

  float leftPortX = 16.0f;
  float topPortY = 245.0f;
  float portSpacing = 43.0f;
  installInput<rack::PJ301MPort>(Stage::STAGE_IN, leftPortX, topPortY);
  installInput<rack::PJ301MPort>(Stage::TRIGGER_IN, leftPortX, topPortY + portSpacing);
  installInput<rack::PJ301MPort>(Stage::DEFER_GATE_IN, leftPortX, topPortY + 2 * portSpacing);

  float rightPortX = widgetRightEdge - leftPortX;
  installOutput<rack::PJ301MPort>(Stage::STAGE_OUT, rightPortX, topPortY);
  installOutput<rack::PJ301MPort>(Stage::EOC_TRIGGER_OUT, rightPortX, topPortY + portSpacing);
  installOutput<rack::PJ301MPort>(Stage::ACTIVE_GATE_OUT, rightPortX, topPortY + 2 * portSpacing);

  installScrews(13.0f, 7.5f);
}
}