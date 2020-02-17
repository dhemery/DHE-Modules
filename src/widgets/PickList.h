#pragma once
#include "Dimensions.h"

#include <app/ParamWidget.hpp>
#include <engine/Module.hpp>
#include <helpers.hpp>
#include <random.hpp>
#include <utility>
#include <widget/FramebufferWidget.hpp>
#include <widget/SvgWidget.hpp>
#include <window.hpp>

namespace dhe {
namespace picklist {

  class Item : public rack::widget::SvgWidget {
  public:
    Item(std::string const &svgDir, std::string const &menuName, int index, std::function<void(int)> onPick) :
        index{index}, onPick{std::move(onPick)} {
      auto const controlName = menuName + "-item-" + std::to_string(index + 1);
      setSvg(controlSvg(svgDir, controlName));
    }

    /**
     * Pass this item's index to <tt>onPick(int)</tt> and hide the parent menu.
     */
    void onAction(rack::event::Action const &action) override {
      SvgWidget::onAction(action);
      onPick(index);
      parent->hide();
      action.consume(this);
    }

    /**
     * Take action if the button event is a left mouse button press.
     */
    void onButton(rack::event::Button const &buttonEvent) override {
      SvgWidget::onButton(buttonEvent);
      buttonEvent.stopPropagating();
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

  class Menu : public rack::widget::SvgWidget {
  public:
    Menu(std::string const &svgDir, std::string const &menuName, int size, std::function<void(int)> const &onPick) {
      auto const controlName = menuName + "-menu";
      setSvg(controlSvg(svgDir, controlName));
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
      SvgWidget::onEnter(entryEvent);
      isHovered = true;
      entryEvent.consume(this);
    }

    /**
     * Consume the hover events so that Rack will report Enter and Leave events.
     */
    void onHover(const rack::event::Hover &hoverEvent) override {
      SvgWidget::onHover(hoverEvent);
      hoverEvent.stopPropagating();
      hoverEvent.consume(this); // registers for Enter and Leave events
    }

    /**
     * Note that the mouse is no longer hovering over this menu.
     */
    void onLeave(const rack::event::Leave &exitEvent) override {
      SvgWidget::onLeave(exitEvent);
      isHovered = false;
      exitEvent.consume(this);
    }

    void draw(const DrawArgs &args) override {
      SvgWidget::draw(args);
      Widget::draw(args); // Because SvgWidget::draw() does not draw children
    }

  private:
    bool isHovered{};
  };

  class Button : public rack::app::ParamWidget {
  public:
    Button(std::string const &svgDir, std::string const &menuName, int size, rack::engine::Module *module, float xmm,
           float ymm, int index) {
      frameBuffer = new rack::widget::FramebufferWidget{};
      addChild(frameBuffer);

      selectedOption = new rack::widget::SvgWidget{};
      frameBuffer->addChild(selectedOption);

      for (int optionIndex = 0; optionIndex < size; optionIndex++) {
        auto const controlName = menuName + "-item-" + std::to_string(optionIndex + 1);
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
      positionCentered(popupMenu, xmm, ymm);
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
      selectedOption->setSvg(optionSvgs[selection]);
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

    /**
     * Don't propagate over events to whatever is behind this button.
     */
    void onHover(const rack::event::Hover &hoverEvent) override {
      ParamWidget::onHover(hoverEvent);
      hoverEvent.stopPropagating();
    }

    void reset() override { paramQuantity->reset(); }

    void randomize() override {
      float value
          = paramQuantity->getMinValue() + std::floor(rack::random::uniform() * (paramQuantity->getRange() + 1));
      paramQuantity->setValue(value);
    }

    auto menu() -> rack::widget::Widget * { return popupMenu; }

  private:
    void addFrame(std::shared_ptr<rack::Svg> const &svg) {
      optionSvgs.push_back(svg);
      if (!selectedOption->svg) {
        selectedOption->setSvg(svg);
        box.size = selectedOption->box.size;
        frameBuffer->box.size = selectedOption->box.size;
      }
    }

    std::vector<std::shared_ptr<rack::Svg>> optionSvgs{};
    rack::widget::FramebufferWidget *frameBuffer{};
    rack::widget::SvgWidget *selectedOption{};
    Menu *popupMenu;
  };

  static inline auto button(std::string const &svgDir, std::string const &name, int size, rack::engine::Module *module,
                            float xmm, float ymm, int index) -> Button * {
    return new Button{svgDir, name, size, module, xmm, ymm, index};
  }
} // namespace picklist

} // namespace dhe
