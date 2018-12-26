require_relative 'button'
require_relative 'control'
require_relative 'dimensions'
require_relative 'label'

module DHE
  class ButtonControl < Control
    DIAMETER = 6.0

    def initialize(module_, options)
      super(module_, options)
      label_text = options[:label]
      @label = Label.new(module_: module_, text: label_text, size: :small)
      @button = Button.new(module_: module_)
    end

    def draw_faceplate(svg:, x:, y:)
      @label.draw_svg(svg: svg, x: x, y: @button.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @button.draw_svg(svg: svg, x: x, y: y)
    end

    def control_files
      [:on, :off].map { |state| @button.svg_file(state: state) }
    end
  end
end