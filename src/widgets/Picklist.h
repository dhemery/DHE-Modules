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
  class Option : public rack::ui::MenuItem {
  public:
    Option(std::string const &label, std::function<void()> onPick) : onPick{std::move(onPick)} { this->text = label; }

    /**
     * When this option is selected, execute the <tt>onPick()</tt> function.
     */
    void onAction(rack::event::Action const &action) override {
      MenuItem::onAction(action);
      onPick();
      action.consume(this);
    }

  private:
    std::function<void()> onPick;
  };

  class Menu : public rack::ui::Menu {
  public:
    /**
     * Hide this menu unless it is active.
     * The menu is "active" while the mouse is hovering over it.
     */
    void hideIfInactive() {
      if (!isActive) {
        hide();
      }
    }

    /**
     * When the mouse position enters this menu, consider the menu active.
     */
    void onEnter(const rack::event::Enter &entryEvent) override {
      Widget::onEnter(entryEvent);
      isActive = true;
      entryEvent.consume(this);
    }

    /**
     * While the mouse hovers over this menu, consume the hover events
     * so that Rack will report Enter and Leave events.
     */
    void onHover(const rack::event::Hover &hoverEvent) override {
      OpaqueWidget::onHover(hoverEvent);
      hoverEvent.consume(this); // registers for Enter and Leave events
    }

    /**
     * When the mouse position leaves this menu, consider the menu inactive.
     */
    void onLeave(const rack::event::Leave &exitEvent) override {
      Widget::onLeave(exitEvent);
      isActive = false;
      exitEvent.consume(this);
    }

  private:
    bool isActive{};
  };

  class Button : public rack::app::ParamWidget {
  public:
    Button(std::string const &listName, std::vector<std::string> const &optionNames, std::string const &moduleSvgDir,
           rack::engine::Module *module, float xmm, float ymm, int index) {
      frameBuffer = new rack::widget::FramebufferWidget{};
      addChild(frameBuffer);

      selectionWidget = new rack::widget::SvgWidget{};
      frameBuffer->addChild(selectionWidget);

      for (size_t i = 0; i < optionNames.size(); i++) {
        static auto const basename = std::string{"picklist-"};
        auto const filename = basename + listName + '-' + std::to_string(i + 1);
        addFrame(controlSvg(moduleSvgDir, filename));
      }

      if (module != nullptr) {
        paramQuantity = module->paramQuantities[index];
      }

      positionCentered(this, xmm, ymm);

      for (size_t i = 0; i < optionNames.size(); i++) {
        auto const onPick = [this, i]() {
          auto const value = static_cast<float>(i);
          paramQuantity->setValue(value);
          popupMenu->hide();
        };
        auto *option = new Option(optionNames[i], onPick);
        popupMenu->addChild(option);
      }

      popupMenu->hide();
      positionCentered(popupMenu, xmm, ymm);
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
      selectionWidget->setSvg(optionSvgs[selection]);
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

    auto menu() -> rack::ui::Menu * { return popupMenu; }

  private:
    void addFrame(std::shared_ptr<rack::Svg> const &svg) {
      optionSvgs.push_back(svg);
      // If this is our first frame, automatically set SVG and size
      if (!selectionWidget->svg) {
        selectionWidget->setSvg(svg);
        box.size = selectionWidget->box.size;
        frameBuffer->box.size = selectionWidget->box.size;
      }
    }

    std::vector<std::shared_ptr<rack::Svg>> optionSvgs{};
    rack::widget::FramebufferWidget *frameBuffer{};
    rack::widget::SvgWidget *selectionWidget{};
    Menu *const popupMenu{new Menu};
  };

  static inline auto button(std::string const &listName, std::vector<std::string> const &optionLabels,
                            std::string const &moduleSvgDir, rack::engine::Module *module, float xmm, float ymm,
                            int index) -> Button * {
    return new Button(listName, optionLabels, moduleSvgDir, module, xmm, ymm, index);
  }
} // namespace picklist

} // namespace dhe
