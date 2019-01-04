require_relative 'module'

module DHE
  class Swave < DHE::Module
    def initialize
      super(name: 'swave', hp: 4, foreground: [16,100,50], background: [16,100,97])

      center = @width / 2.0

      large_knob(x: center, y: 43.5, label: 'curve')
      cv_port(x: center, y: 62)
      input_port(x: center, y: 97)
      output_port(x: center, y: 112)
    end
  end
end

