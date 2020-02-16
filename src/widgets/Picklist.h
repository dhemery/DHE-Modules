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
    Item(std::string const &svgDir, std::string const &menuName, unsigned int index, std::function<void(int)> onPick) :
        index{index}, onPick{std::move(onPick)} {
      auto const filename = menuName + "-item-" + std::to_string(index + 1);
      setSvg(controlSvg(svgDir, filename));
    }

    /**
     * When selected, execute the <tt>onPick()</tt> function and hide the parent menu.
     */
    void onAction(rack::event::Action const &action) override {
      SvgWidget::onAction(action);
      onPick(index);
      parent->hide();
      action.consume(this);
    }

  private:
    unsigned int const index;
    std::function<void(int)> onPick;
  };

  class Menu : public rack::widget::SvgWidget {
  public:
    Menu(std::string const &svgDir, std::string const &menuName, std::vector<std::string> const &optionNames,
         std::function<void(int)> const &onPick) {
      auto const filename = menuName + "-menu";
      auto nextItemTop = 0.F;
      for (unsigned int optionIndex = 0; optionIndex < optionNames.size(); optionIndex++) {
        auto *item = new Item{svgDir, menuName, optionIndex, onPick};
        addChild(item);
        item->box.pos.y = nextItemTop;
        nextItemTop += item->box.size.y;
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
     * Note when the mouse begins hovering over this menu.
     */
    void onEnter(const rack::event::Enter &entryEvent) override {
      SvgWidget::onEnter(entryEvent);
      isHovered = true;
      entryEvent.consume(this);
    }

    /**
     * While the mouse hovers over this menu, consume the hover events
     * so that Rack will report Enter and Leave events.
     */
    void onHover(const rack::event::Hover &hoverEvent) override {
      SvgWidget::onHover(hoverEvent);
      hoverEvent.consume(this); // registers for Enter and Leave events
    }

    /**
     * Note when the mouse stops hovering over this menu.
     */
    void onLeave(const rack::event::Leave &exitEvent) override {
      SvgWidget::onLeave(exitEvent);
      isHovered = false;
      exitEvent.consume(this);
    }

  private:
    bool isHovered{};
  };

  class Button : public rack::app::ParamWidget {
  public:
    Button(std::string const &svgDir, std::string const &menuName, std::vector<std::string> const &optionNames,
           rack::engine::Module *module, float xmm, float ymm, int index) {
      frameBuffer = new rack::widget::FramebufferWidget{};
      addChild(frameBuffer);

      selectedOption = new rack::widget::SvgWidget{};
      frameBuffer->addChild(selectedOption);

      for (size_t optionIndex = 0; optionIndex < optionNames.size(); optionIndex++) {
        auto const filename = menuName + "-button-" + std::to_string(optionIndex + 1);
        addFrame(controlSvg(svgDir, filename));
      }

      if (module != nullptr) {
        paramQuantity = module->paramQuantities[index];
      }

      positionCentered(this, xmm, ymm);

      auto const onPick = [this](int optionIndex) {
        auto const value = static_cast<float>(optionIndex);
        paramQuantity->setValue(value);
      };

      popupMenu = new Menu{svgDir, menuName, optionNames, onPick};
    }

    /**
     * When the button is pressed, toggle the visibility of the menu.
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
     * If the button event is a left mouse button press, generate a toggle menu visibility action.
     */
    void onButton(rack::event::Button const &buttonEvent) override {
      ParamWidget::onButton(buttonEvent);
      static auto const toggleMenuVisibility = rack::event::Action{};

      if (buttonEvent.action == GLFW_PRESS && buttonEvent.button == GLFW_MOUSE_BUTTON_LEFT) {
        onAction(toggleMenuVisibility);
        buttonEvent.consume(this); // registers for Deselect events
      }
    }

    /**
     * When the param value changes, display the newly selected option.
     */
    void onChange(rack::event::Change const &changeEvent) override {
      auto const selection = static_cast<int>(paramQuantity->getValue());
      selectedOption->setSvg(optionSvgs[selection]);
      frameBuffer->dirty = true;
      ParamWidget::onChange(changeEvent);
      changeEvent.consume(this);
    }

    /**
     * When another widget is selected, dismiss the menu unless it is the newly selected widget.
     */
    void onDeselect(const rack::event::Deselect &deselectionEvent) override {
      Widget::onDeselect(deselectionEvent);
      popupMenu->hideIfInactive();
      deselectionEvent.consume(this);
    }

    /**
     * When this button is hidden, also dismiss the menu.
     */
    void onHide(const rack::event::Hide &hideEvent) override {
      popupMenu->hide();
      Widget::onHide(hideEvent);
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
      // If this is our first frame, automatically set SVG and size
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

  static inline auto button(std::string const &svgDir, std::string const &listName,
                            std::vector<std::string> const &optionLabels, rack::engine::Module *module, float xmm,
                            float ymm, int index) -> Button * {
    return new Button{svgDir, listName, optionLabels, module, xmm, ymm, index};
  }
} // namespace picklist

} // namespace dhe
