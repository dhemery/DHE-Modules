require_relative 'control'
require_relative 'dimensions'
require_relative 'knob'
require_relative 'label'

module DHE
  class KnobControl < Control
    def initialize(module_, options)
      super(module_, options)
      label_text = options[:label]
      @style = options.fetch(:style, :large)
      @label = Label.new(module_: module_, text: label_text, size: :large)
      @knob = Knob.new(module_: module_, size: @style.to_sym)
    end

    def draw_faceplate(svg:, x:, y:)
      @label.draw_svg(svg: svg, x: x, y: @knob.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @knob.draw_svg(svg: svg, x: x, y: y)
    end

    def control_files
      [@knob.svg_file]
    end
  end
end