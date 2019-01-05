require_relative 'module'

module DHE
  class Xycloid < DHE::Module
    def initialize
      super(name: 'XYCLOID', hp: 11, foreground: [270,100,50], background: [270,66,97])

      left = @width / 7.0
      right = @width - left
      left_center = (right - left) / 3.0 + left
      right_center = @width - left_center

      y = 30.0
      delta_y = 22.0
      port_offset = 1.25

      connector(x1: left, x2: right, y1: y)
      cv_port(x: left, y: y)
      attenuverter(x: left_center, y: y)
      large_knob(x: right_center, y: y, label: 'RATIO')
      toggle(x: right, y: y, labels: %w(LOCK FREE), position: 2)

      y += delta_y

      connector(x1: left, x2: right, y1: y)
      cv_port(x: left, y: y)
      attenuverter(x: left_center, y: y)
      large_knob(x: right_center, y: y, label: 'DEPTH')
      toggle(x: right, y: y, labels: ['IN', ' ', 'OUT'], position: 2)

      y += delta_y

      connector(x1: left, x2: right, y1: y)
      cv_port(x: left, y: y)
      attenuverter(x: left_center, y: y)
      large_knob(x: right_center, y: y, label: 'SPEED')
      small_knob(x: right, y: y, label: 'PHASE')

      y = 97.0
      delta_y = 15.0

      connector(x1: left, x2: right, y1: y)
      cv_port(x: left, y: y)
      small_knob(x: left_center, y: y, label: 'GAIN')
      polarity_toggle(x: right_center, y: y)
      output_port(x: right, y: y + port_offset, label: 'X OUT')

      y += delta_y

      connector(x1: left, x2: right, y1: y)
      cv_port(x: left, y: y)
      small_knob(x: left_center, y: y, label: 'GAIN')
      polarity_toggle(x: right_center, y: y)
      output_port(x: right, y: y + port_offset, label: 'Y OUT')
    end
  end
end
