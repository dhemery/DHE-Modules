require_relative 'shape'
require_relative 'label'
require_relative '../dimensions'

module PickList
  STROKE_WIDTH = 0.25
  CORNER_RADIUS = STROKE_WIDTH * 2

  class Option < CenteredShape
    attr_reader :slug

    def initialize(name:, text:, color:, border:, fill:, width:, position:, type:)
      @label = Label.new(color: color, alignment: :center, size: :small, text: text)
      height = @label.height + 2 * PADDING
      super(width: width, height: height)

      @slug = Pathname("#{name}-#{type}-#{position}")
      @border = border
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
                  stroke: @border, fill: @fill)
      @label.draw(canvas)
    end

    def has_text?
      true
    end
  end

  class Button < Option
    def initialize(name:, text:, color:, fill:, width:, position:)
      super(
        name: name, text: text, position: position,
        color: color, fill: fill, border: fill,
        width: width, type: 'button'
      )
    end
  end

  class Item < Option
    def initialize(name:, text:, color:, fill:, width:, position:)
      super(
        name: name, text: text, position: position,
        color: color, fill: fill, border: color,
        width: width, type: 'item'
      )
    end
  end

  class Menu < CenteredShape
    attr_reader :slug

    def initialize(name:, border:, fill:, width:, height:)
      super(width: width, height: height)

      @stroke = border
      @fill = fill
      @slug = Pathname("#{name}-menu")
    end

    def draw(canvas)
      inner_height = height - STROKE_WIDTH
      inner_width = width - STROKE_WIDTH
      inner_top = inner_height / -2.0
      inner_left = inner_width / -2.0
      canvas.rect(x: inner_left, width: inner_width, y: inner_top, height: inner_height,
                  rx: CORNER_RADIUS, ry: CORNER_RADIUS,
                  'stroke-width' => STROKE_WIDTH,
                  stroke: @stroke, fill: @fill)
    end
  end
end
