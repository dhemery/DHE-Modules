require_relative 'module'

module DHE
  class Stage < DHE::Module
    def initialize
      super(name: 'stage', hp: 5, foreground: [120, 100, 30], background: [120, 100, 97])

      left = @width / 4.0 + 1.0 / 3.0
      right = @width - left
      center = @width / 2.0

      y = 25.0
      delta_y = 18.5

      large_knob(x: center, y: y, label: 'level')

      y += delta_y

      large_knob(x: center, y: y, label: 'curve')

      y += delta_y

      large_knob(x: center, y: y, label: 'duration')

      y = 82.0
      delta_y = 15.0

      input_port(x: left, y: y, label: 'defer')
      output_port(x: right, y: y, label: 'active')

      y += delta_y

      input_port(x: left, y: y, label: 'trig')
      output_port(x: right, y: y, label: 'eoc')

      y += delta_y

      input_port(x: left, y: y, label: 'in')
      output_port(x: right, y: y, label: 'out')
    end
  end
end
