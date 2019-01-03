require_relative 'control'
require_relative '../shapes/dimensions'
require_relative '../shapes/knob'
require_relative '../shapes/label'

module DHE
  class KnobControl < Control
    def initialize(module_, options)
      super(module_, options)
      style = options[:style]
      @size = to_size(style)
      label_text = options[:label] || default_label(style)

      @label = Label.new(module_: module_, text: label_text, size: :large)
      @knob = Knob.new(module_: module_, size: @size.to_sym)
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

    private

    def to_size(style)
      case style
        when 'av'
          :tiny
        when nil
          :large
        else
          style.to_sym
      end
    end

    def default_label(style)
      case style
        when 'av'
          '- +'
        else
          ''
      end
    end
  end
end