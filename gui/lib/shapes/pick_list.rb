require_relative 'shape'
require_relative 'label'
require_relative '../dimensions'

module PickList

  class Item < CenteredShape
    STROKE_WIDTH = 0.25
    CORNER_RADIUS = STROKE_WIDTH * 2
    attr_reader :slug

    def initialize(name:, text:, color:, fill:, width:, position:)
      @color = color
      @label = Label.new(color: @color, alignment: :center, size: :small, text: text)
      height = @label.height + 2 * PADDING
      super(width: width, height: height)

      @slug = Pathname("#{name}-#{position}")
      @fill = fill
    end

    def draw(canvas)
      inner_height = height - STROKE_WIDTH
      inner_width = width - STROKE_WIDTH
      inner_top = inner_height / -2.0
      inner_left = inner_width / -2.0
      canvas.rect(x: inner_left, y: inner_top, width: inner_width, height: inner_height,
                  rx: CORNER_RADIUS, ry: CORNER_RADIUS,
                  'stroke-width' => STROKE_WIDTH,
                  stroke: @color, fill: @fill)
      @label.draw(canvas)
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

      @color = color
      @slug = Pathname("#{name}-menu")
    end

    def draw(canvas)
      canvas.rect(x: left, y: top, width: width, height: height,
                  rx: CORNER_RADIUS, ry: CORNER_RADIUS,
                  stroke: 'none', fill: @color)
    end
  end
end
