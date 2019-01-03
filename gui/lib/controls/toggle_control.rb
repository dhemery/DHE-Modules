require_relative 'control'
require_relative '../shapes/dimensions'
require_relative '../shapes/label'
require_relative '../shapes/toggle'

module DHE
  class ToggleControl < Control
    def initialize(module_, options)
      super(module_, options)
      style = options[:style]
      label_texts = options[:labels] || labels_for(style)
      @selection = options[:selection] || selection_for(style)
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
      (1..@toggle.size).map { |position| @toggle.svg_file(position: position) }
    end

    private

    def labels_for(style)
      case style
        when 'level'
          %w(bi uni)
        when 'shape'
          %w(j s)
        when 'duration'
          %w(1 10 100)
        else
          []
      end
    end

    def selection_for(style)
      case style
        when 'duration'
          2
        else
          1
      end
    end
  end
end