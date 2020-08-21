#pragma once
#include "Dimensions.h"

#include <app/ParamWidget.hpp>
#include <engine/Module.hpp>
#include <functional>
#include <helpers.hpp>
#include <random.hpp>
#include <utility>
#include <widget/FramebufferWidget.hpp>
#include <widget/SvgWidget.hpp>
#include <window.hpp>

namespace dhe {
namespace picklist {

class Item : public rack::widget::OpaqueWidget {
public:
  Item(std::string const &svgDir, std::string const &menuName, int index,
       std::function<void(int)> onPick)
      : index{index}, onPick{std::move(onPick)} {
    auto const controlName = menuName + '-' + std::to_string(index + 1);
    auto *label = new rack::widget::SvgWidget{};
    label->setSvg(controlSvg(svgDir, controlName));
    addChild(label);
    setSize(label->box.size);
  }

  /**
   * Pass this item's index to <tt>onPick(int)</tt> and hide the parent menu.
   */
  void onAction(rack::event::Action const &action) override {
    OpaqueWidget::onAction(action);
    onPick(index);
    parent->hide();
    action.consume(this);
  }

  /**
   * Take action if the button event is a left mouse button press.
   */
  void onButton(rack::event::Button const &buttonEvent) override {
    OpaqueWidget::onButton(buttonEvent);
    if (buttonEvent.button == GLFW_MOUSE_BUTTON_LEFT) {
      if (buttonEvent.action == GLFW_PRESS) {
        onAction(rack::event::Action{});
      }
      buttonEvent.consume(this);
    }
  }

private:
  int const index;
  std::function<void(int)> onPick;
};

class Menu : public rack::widget::OpaqueWidget {
public:
  Menu(std::string const &svgDir, std::string const &menuName, int size,
       std::function<void(int)> const &onPick) {
    auto const controlName = menuName + "-menu";
    auto *background = new rack::widget::SvgWidget{};
    background->setSvg(controlSvg(svgDir, controlName));
    addChild(background);
    setSize(background->box.size);
    auto const inset = mm2px(0.5F);
    auto itemTop = inset;
    for (auto optionIndex = 0; optionIndex < size; optionIndex++) {
      auto *item = new Item{svgDir, menuName, optionIndex, onPick};
      item->setPosition({inset, itemTop});
      addChild(item);
      itemTop += item->box.size.y;
    }
    hide();
  }

  /**
   * Hide unless the mouse is hovering over this menu.
   */
  void hideIfInactive() {
    if (!isHovered) {
      hide();
    }
  }

  /**
   * Note that the mouse is now hovering over this menu.
   */
  void onEnter(const rack::event::Enter &entryEvent) override {
    OpaqueWidget::onEnter(entryEvent);
    isHovered = true;
    entryEvent.consume(this);
  }

  /**
   * Consume the hover events so that Rack will report Enter and Leave events.
   */
  void onHover(const rack::event::Hover &hoverEvent) override {
    OpaqueWidget::onHover(hoverEvent);
    hoverEvent.consume(this); // registers for Enter and Leave events
  }

  /**
   * Note that the mouse is no longer hovering over this menu.
   */
  void onLeave(const rack::event::Leave &exitEvent) override {
    OpaqueWidget::onLeave(exitEvent);
    isHovered = false;
    exitEvent.consume(this);
  }

private:
  bool isHovered{};
};

class Button : public rack::app::ParamWidget {
public:
  Button(std::string const &svgDir, std::string const &menuName, int size,
         rack::engine::Module *module, float xmm, float ymm, int index) {
    frameBuffer = new rack::widget::FramebufferWidget{};
    addChild(frameBuffer);

    label = new rack::widget::SvgWidget{};
    frameBuffer->addChild(label);

    for (int optionIndex = 0; optionIndex < size; optionIndex++) {
      auto const controlName = menuName + '-' + std::to_string(optionIndex + 1);
      addFrame(controlSvg(svgDir, controlName));
    }

    if (module != nullptr) {
      paramQuantity = module->paramQuantities[index];
    }

    positionCentered(this, xmm, ymm);

    auto const onPick = [this](int optionIndex) {
      auto const value = static_cast<float>(optionIndex);
      paramQuantity->setValue(value);
    };

    popupMenu = new Menu{svgDir, menuName, size, onPick};
    popupMenu->setPosition({box.pos.x, box.pos.y + box.size.y});
  }

  /**
   * Toggle the menu visibility.
   */
  void onAction(rack::event::Action const &toggleMenuVisibility) override {
    ParamWidget::onAction(toggleMenuVisibility);
    if (popupMenu->visible) {
      popupMenu->hide();
    } else {
      popupMenu->show();
    }
    toggleMenuVisibility.consume(this);
  }

  /**
   * Take action if the button event is a left mouse button press.
   */
  void onButton(rack::event::Button const &buttonEvent) override {
    ParamWidget::onButton(buttonEvent);

    buttonEvent.stopPropagating();
    if (buttonEvent.button == GLFW_MOUSE_BUTTON_LEFT) {
      if (buttonEvent.action == GLFW_PRESS) {
        onAction(rack::event::Action{});
      }
      buttonEvent.consume(this);
    }
  }

  /**
   * Display the newly selected option.
   */
  void onChange(rack::event::Change const &changeEvent) override {
    auto const selection = static_cast<int>(paramQuantity->getValue());
    label->setSvg(labelSvgs[selection]);
    frameBuffer->dirty = true;
    ParamWidget::onChange(changeEvent);
    changeEvent.consume(this);
  }

  /**
   * Dismiss the menu unless it now selected.
   */
  void onDeselect(const rack::event::Deselect &deselectionEvent) override {
    ParamWidget::onDeselect(deselectionEvent);
    popupMenu->hideIfInactive();
    deselectionEvent.consume(this);
  }

  /**
   * Dismiss the menu.
   */
  void onHide(const rack::event::Hide &hideEvent) override {
    ParamWidget::onHide(hideEvent);
    popupMenu->hide();
  }

  void reset() override { paramQuantity->reset(); }

  void randomize() override {
    float value =
        paramQuantity->getMinValue() +
        std::floor(rack::random::uniform() * (paramQuantity->getRange() + 1));
    paramQuantity->setValue(value);
  }

  auto menu() -> rack::widget::Widget * { return popupMenu; }

private:
  void addFrame(std::shared_ptr<rack::Svg> const &svg) {
    labelSvgs.push_back(svg);
    if (!label->svg) {
      label->setSvg(svg);
      setSize(label->box.size);
      frameBuffer->setSize(label->box.size);
    }
  }

  std::vector<std::shared_ptr<rack::Svg>> labelSvgs{};
  rack::widget::FramebufferWidget *frameBuffer{};
  rack::widget::SvgWidget *label{};
  Menu *popupMenu;
};

static inline auto button(std::string const &svgDir, std::string const &name,
                          int size, rack::engine::Module *module, float xmm,
                          float ymm, int index) -> Button * {
  return new Button{svgDir, name, size, module, xmm, ymm, index};
}
} // namespace picklist

} // namespace dhe
