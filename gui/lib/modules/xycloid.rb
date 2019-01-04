require_relative 'module'

module DHE
  class Xycloid < DHE::Module
    def initialize
      super(name: 'xycloid', hp: 11, foreground: [270,100,50], background: [270,66,97])

      left = @width / 7.0
      right = @width - left
      left_center = (right - left) / 3.0 + left
      right_center = @width - left_center

      y = 30.0
      delta_y = 22.0

      connector(x1: left, x2: right, y1: y)
      cv_port(x: left, y: y)
      attenuverter(x: left_center, y: y)
      large_knob(x: right_center, y: y, label: 'ratio')
      toggle(x: right, y: y, labels: %w(lock free), position: 2)

      y += delta_y

      connector(x1: left, x2: right, y1: y)
      cv_port(x: left, y: y)
      attenuverter(x: left_center, y: y)
      large_knob(x: right_center, y: y, label: 'depth')
      toggle(x: right, y: y, labels: ['in', ' ', 'out'], position: 2)

      y += delta_y

      connector(x1: left, x2: right, y1: y)
      cv_port(x: left, y: y)
      attenuverter(x: left_center, y: y)
      large_knob(x: right_center, y: y, label: 'speed')
      small_knob(x: right, y: y, label: 'phase')

      y = 97.0
      delta_y = 15.0

      connector(x1: left, x2: right, y1: y)
      cv_port(x: left, y: y)
      small_knob(x: left_center, y: y, label: 'gain')
      polarity_toggle(x: right_center, y: y)
      output_port(x: right, y: y, label: 'x out')

      y += delta_y

      connector(x1: left, x2: right, y1: y)
      cv_port(x: left, y: y)
      small_knob(x: left_center, y: y, label: 'gain')
      polarity_toggle(x: right_center, y: y)
      output_port(x: right, y: y, label: 'x out')
    end
  end
end


