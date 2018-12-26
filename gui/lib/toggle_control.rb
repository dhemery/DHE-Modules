require_relative 'control'
require_relative 'dimensions'
require_relative 'label'
require_relative 'toggle'

module DHE
  class ToggleControl < Control
    def initialize(module_, options)
      super(module_, options)
      label_texts = options[:labels]
      @selection = options.fetch(:selection, 1)
      @labels = Array(Label.new(module_: module_, text: label_texts.first, size: :small, alignment: :below))
      @labels << Label.new(module_: module_, text: label_texts[1], size: :small, alignment: :right_of) if (label_texts.size == 3)
      @labels << Label.new(module_: module_, text: label_texts.last, size: :small)
      @toggle = Toggle.new(module_: module_, size: label_texts.size)
    end

    def draw_faceplate(svg:, x:, y:)
      @labels.first.draw_svg(svg: svg, x: x, y: @toggle.bottom(y + PADDING))
      @labels[1].draw_svg(svg: svg, x: @toggle.right(x + PADDING / 2), y: y) if (@labels.size == 3)
      @labels.last.draw_svg(svg: svg, x: x, y: @toggle.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @toggle.draw_svg(svg: svg, x: x, y: y, position: @selection)
    end

    def control_files
      (1..@toggle.size).map {|position| @toggle.svg_file(position: position)}
    end
  end
end