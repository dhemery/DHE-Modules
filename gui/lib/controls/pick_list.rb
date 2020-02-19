require_relative '../dimensions'
require_relative '../shapes/label'
require_relative '../shapes/box'

module PickList

  class Item < CenteredShape
    STROKE_WIDTH = 0.25
    CORNER_RADIUS = STROKE_WIDTH * 2
    attr_reader :slug

    def initialize(name:, text:, color:, fill:, width:, position:)
      @slug = Pathname("#{name}-#{position}")

      label = Label.new(color: color, alignment: :center, size: :small, text: text)
      height = label.height + 2 * PADDING
      super(width: width, height: height)

      bottom = (height - STROKE_WIDTH) / 2
      right = (width - STROKE_WIDTH) / 2

      box = Box.new(top: -bottom, right: right, bottom: bottom, left: -right,
                    fill: fill, stroke: color, corner_radius: CORNER_RADIUS, stroke_width: STROKE_WIDTH)
      @shapes = [box, label]
    end

    def draw(canvas)
      @shapes.each { |shape| shape.draw(canvas) }
    end

    def has_text?
      true
    end
  end

  class Menu < CenteredShape
    CORNER_RADIUS = 1.0
    attr_reader :slug

    def initialize(name:, color:, width:, height:)
      super(width: width, height: height)
      @slug = Pathname("#{name}-menu")

      @shapes = [
        Box.new(top: top, right: right, bottom: bottom, left: left, fill: color, corner_radius: CORNER_RADIUS)
      ]
    end

    def draw(canvas)
      @shapes.each { |shape| shape.draw(canvas) }
    end
  end
end
