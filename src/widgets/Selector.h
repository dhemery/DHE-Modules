#pragma once
#include "Dimensions.h"

#include <app/ParamWidget.hpp>
#include <engine/Module.hpp>
#include <helpers.hpp>
#include <ui/Label.hpp>
#include <utility>

namespace dhe {
class Selector : public rack::app::ParamWidget {
public:
  Selector(rack::engine::Module *module, const std::vector<std::string> &options, float x, float y, int index) {
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }

    setSize(rack::math::Vec{hp2px(2.F), hp2px(1.F)});
    positionCentered(this, x, y);
    menu = new rack::ui::Menu;
    menu->hide();
    addChild(menu);
    for (auto const &option : options) {
      labels.push_back(makeLabel(option));
      auto *item = rack::createMenuItem(option, "foo");
      menu->addChild(item);
    }
    selectedLabel = labels[0];
  }

  void draw(const DrawArgs &args) override {
    rack::app::ParamWidget::draw(args);
    selectedLabel->draw(args);
  }

  void onAction(rack::event::Action const &action) override {
    if (menu->visible) {
      menu->hide();
    } else {
      menu->show();
    }
    action.consume(this);
  }

  void select(int selection) {
    selectedLabel = labels[selection];
    paramQuantity->setValue(static_cast<float>(selection));
  }

  void onButton(rack::event::Button const &buttonEvent) override {
    static auto const buttonPressed = rack::event::Action{};

    if (buttonEvent.action == GLFW_PRESS && buttonEvent.button == GLFW_MOUSE_BUTTON_LEFT) {
      onAction(buttonPressed);
      buttonEvent.consume(this);
    }
  }

private:
  rack::ui::Menu *menu;
  std::vector<rack::ui::Label *> labels{};
  rack::ui::Label *selectedLabel;

  static auto makeLabel(std::string text) -> rack::ui::Label * {
    auto *label = new rack::ui::Label;
    label->text = std::move(text);
    label->color = nvgRGBf(1.F, 0.F, 0.F);
    return label;
  }
};

} // namespace dhe
