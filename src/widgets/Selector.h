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
  static inline auto button(rack::engine::Module *module, const std::vector<std::string> &options, float xmm,
                                float ymm, int index) -> Selector * {
    return new Selector(module, options, xmm, ymm, index);
  }

  Selector(rack::engine::Module *module, const std::vector<std::string> &options, float xmm, float ymm, int index) {
    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }

    setSize(rack::math::Vec{hp2px(2.F), hp2px(1.F)});
    positionCentered(this, xmm, ymm);
    popupMenu = new rack::ui::Menu;
    popupMenu->hide();
    positionCentered(popupMenu, xmm, ymm);
    for (auto const &option : options) {
      labels.push_back(makeLabel(option));
      auto *item = rack::createMenuItem(option, "foo");
      popupMenu->addChild(item);
    }
    selectedLabel = labels[0];
  }

  void draw(const DrawArgs &args) override {
    rack::app::ParamWidget::draw(args);
    selectedLabel->draw(args);
  }

  void onAction(rack::event::Action const &action) override {
    if (popupMenu->visible) {
      popupMenu->hide();
    } else {
      popupMenu->show();
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

  auto menu() -> rack::ui::Menu * { return popupMenu; }

private:
  rack::ui::Menu *popupMenu;
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
