require_relative '../dimensions'
require_relative '../shapes/label'
require_relative '../shapes/box'

class Stepper
  attr_reader :options

  def initialize(name:, options:, text_color:, fill:, width:)
    @options = options.each_with_index.map do |option, index|
      Option.new(name: name, index: index, text_color: text_color, fill: fill, label: option, width: width)
    end
  end

  def frames
    options
  end

  class Option < Shape
    STROKE_WIDTH = 0.25
    CORNER_RADIUS = STROKE_WIDTH * 2
    attr_reader :slug

    def initialize(name:, index:, text_color:, fill:, label:, width:)
      @slug =  Pathname("stepper-#{name}-#{index + 1}")
      @label = label
      label = Label.new(color: text_color, alignment: :center, size: :small, text: label, width: width)
                   .padded(vertical: PADDING)

      box = Box.around(shapes: [label],
                       fill: fill, stroke: text_color, corner_radius: CORNER_RADIUS, stroke_width: STROKE_WIDTH)
      super(top: box.top, right: box.right, bottom: box.bottom, left: box.left)
      @shapes = [box, label]
    end

    def draw(canvas)
      @shapes.each { |shape| shape.draw(canvas) }
    end

    def has_text?
      true
    end
  end
end
