require_relative 'module'

module DHE
  class Upstage < DHE::Module
    def initialize
      super(name: 'upstage', hp: 5, foreground: [210,100,30], background: [210,100,97])

      left = @width / 4.0 + 1.0 / 3.0
      right = @width - left
      center = @width / 2.0

      y = 25.0
      delta_y = 18.5

      large_knob(x: center, y: y, label: 'level')

      y += delta_y
      cv_port(x: left, y: y)
      polarity_toggle(x: right, y: y, position: 2)

      y += delta_y
      button(x: left, y: y, label: 'wait')
      button(x: right, y: y, label: 'trig')

      y = 82.0
      delta_y = 15.0

      input_port(x: left, y: y, label: 'wait')

      y += delta_y
      input_port(x: left, y: y, label: 'trig')
      output_port(x: right, y: y, label: 'trig')

      y += delta_y
      output_port(x: right, y: y)
    end
  end
end
