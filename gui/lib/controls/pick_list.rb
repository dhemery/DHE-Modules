require_relative '../dimensions'
require_relative '../shapes/label'
require_relative '../shapes/box'

module PickList

  class Item < Shape
    STROKE_WIDTH = 0.25
    CORNER_RADIUS = STROKE_WIDTH * 2
    attr_reader :slug

    def initialize(name:, text:, text_color:, fill:, width:, position:)
      @slug = Pathname("#{name}-#{position}")

      label = Label.new(color: text_color, alignment: :center, size: :small, text: text)
      height = label.height + 2 * PADDING
      super(top: -height / 2.0, right: width / 2.0, bottom: height / 2.0, left: -width / 2.0)

      box = Box.centered(height: height - STROKE_WIDTH, width: width - STROKE_WIDTH,
                         fill: fill, stroke: text_color, corner_radius: CORNER_RADIUS, stroke_width: STROKE_WIDTH)
      @shapes = [box, label]
    end

    def draw(canvas)
      @shapes.each { |shape| shape.draw(canvas) }
    end

    def has_text?
      true
    end
  end

  class Menu < Shape
    CORNER_RADIUS = 1.0
    attr_reader :slug

    def initialize(name:, color:, width:, height:)
      super(top: -height / 2.0, right: width / 2.0, bottom: height / 2.0, left: -width / 2.0)
      @slug = Pathname("#{name}-menu")

      @shapes = [
        Box.new(top: top, right: right, bottom: bottom, left: left,
                fill: color, corner_radius: CORNER_RADIUS,
                stroke: :none, stroke_width: 0,
        )
      ]
    end

    def draw(canvas)
      @shapes.each { |shape| shape.draw(canvas) }
    end
  end
end
