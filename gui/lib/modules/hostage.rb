require_relative 'module'

module DHE
  class Hostage < DHE::Module
    def initialize
      super(name: 'hostage', hp: 5, foreground: [300, 100, 30], background: [300, 100, 97])

      left = @width / 4.0 + 1.0 / 3.0
      right = @width - left
      center = @width / 2.0

      y = 25.0
      delta_y = 18.5

      toggle(x: center, y: y, labels: %w(hold sustain), position: 1)

      y += delta_y
      cv_port(x: left, y: y)
      duration_toggle(x: right, y: y)

      y += delta_y
      large_knob(x: center, y: y, label: 'duration')

      y = 82.0
      delta_y = 15.0

      input_port(x: left, y: y, label: 'defer')
      output_port(x: right, y: y, label: 'active')

      y += delta_y
      input_port(x: left, y: y, label: 'gate')
      output_port(x: right, y: y, label: 'eoc')

      y += delta_y
      input_port(x: left, y: y)
      output_port(x: right, y: y)
    end
  end
end
