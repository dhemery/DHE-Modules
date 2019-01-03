require_relative 'module'
require_relative '../shapes/box'
require_relative '../shapes/button'

module DHE
  class BoosterStage < DHE::Module

    def initialize
      super(name: 'booster stage', hp: 8, foreground: [0, 100, 30], background: [0, 100, 97])
      left = @width / 6.0 + 1.0 / 3.0
      right = @width - left
      center = @width / 2.0

      y = 25.0
      delta_y = 18.5

      connector(x1: left, y1: y, x2: right, y2: y)
      cv_port(x: left, y: y)
      large_knob(x: center, y: y, label: 'level')
      polarity_switch(x: right, y: y)

      y += delta_y

      connector(x1: left, y1: y, x2: right, y2: y)
      cv_port(x: left, y: y)
      large_knob(x: center, y: y, label: 'curve')
      shape_switch(x: right, y: y)

      y += delta_y

      connector(x1: left, y1: y, x2: right, y2: y)
      cv_port(x: left, y: y)
      large_knob(x: center, y: y, label: 'duration')
      duration_switch(x: right, y: y)

      y = 82.0
      delta_y = 15.0

      input_button_port(x: left, y: y, label: 'defer')
      output_button_port(x: right, y: y, label: 'active')

      y += delta_y

      input_button_port(x: left, y: y, label: 'trig')
      output_button_port(x: right, y: y, label: 'eoc')

      y += delta_y

      input_port(x: left, y: y, label: 'in')
      output_port(x: right, y: y, label: 'out')
    end

    def input_button_port(x:, y:, label:)
      port = Port.new(faceplate: self, x: x, y: y)
      label = Label.new(faceplate: self, text: label, size: :small, x: x, y: port.top)
      button_x = port.right + PADDING + Button::DIAMETER / 2.0
      button = Button.new(faceplate: self, x: button_x, y: y)
      @faceplate_items << Box.new(faceplate: self, top: label.top, right: button.right, bottom: port.bottom, left: port
                                                                                                                   .left)
      @faceplate_items << label
      @controls << port
      @controls << button
    end

    def output_button_port(x:, y:, label:)
      port = Port.new(faceplate: self, x: x, y: y)
      label = Label.new(faceplate: self, text: label, size: :small, x: x, y: port.top, style: :reversed)
      button_x = port.left - PADDING - Button::DIAMETER / 2.0
      button = Button.new(faceplate: self, x: button_x, y: y, style: :reversed)
      @faceplate_items << Box.new(faceplate: self, style: :reversed, top: label.top, right: port.right, bottom: port
                                                                                                                    .bottom, left: button.left)
      @faceplate_items << label
      @controls << port
      @controls << button
    end
  end
end
