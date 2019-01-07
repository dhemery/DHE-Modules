require_relative '../module'

module DHE
  class Blossom < DHE::Module
    def initialize
      super(name: 'BLOSSOM', hp: 11, foreground: [270, 100, 50], background: [270, 66, 97])
      left = @width / 7.0
      right = @width - left
      center = @width / 2.0
      left_center = (right - left) / 3.0 + left
      right_center = @width - left_center

      y = 25.0
      delta_y = 18.5
      port_offset = 1.25

      connector(left: left, right: right_center, y: y)
      cv_port(x: left, y: y)
      attenuverter(x: left_center, y: y)
      large_knob(x: right_center, y: y, label: 'SPIN')

      y += delta_y

      connector(left: left, right: right, y: y)
      cv_port(x: left, y: y)
      attenuverter(x: left_center, y: y)
      large_knob(x: right_center, y: y, label: 'BOUNCE')
      toggle(x: right, y: y, labels: %w(LOCK FREE), selection: 2)

      y += delta_y

      connector(left: left, right: right_center, y: y)
      cv_port(x: left, y: y)
      attenuverter(x: left_center, y: y)
      large_knob(x: right_center, y: y, label: 'DEPTH')

      y += delta_y

      connector(left: left, right: right_center, y: y)
      cv_port(x: left, y: y)
      attenuverter(x: left_center, y: y)
      large_knob(x: right_center, y: y, label: 'PHASE')

      y = 97.0
      delta_y = 15.0

      connector(left: left, right: right, y: y)
      cv_port(x: left, y: y)
      small_knob(x: left_center, y: y, label: 'GAIN')
      polarity_toggle(x: right_center, y: y)
      output_port(x: right, y: y + port_offset, label: 'X OUT')

      y += delta_y

      connector(left: left, right: right, y: y)
      cv_port(x: left, y: y)
      small_knob(x: left_center, y: y, label: 'GAIN')
      polarity_toggle(x: right_center, y: y)
      output_port(x: right, y: y + port_offset, label: 'Y OUT')
    end

    def input_button_port(x:, y:, label:)
      port = Port.new(faceplate: self, x: x, y: y)
      label = Label.new(faceplate: self, text: label, size: :small, x: x, y: port.top - PADDING)
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
      label = Label.new(faceplate: self, text: label, size: :small, x: x, y: port.top - PADDING, style: :reversed)
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

MODULE_TO_FILENAME[DHE::Blossom.new] = __FILE__
