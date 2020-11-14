require_relative '../dimensions'
require_relative '../shapes/label'
require_relative '../shapes/box'

class PickList
  attr_reader :options, :menu

  def initialize(name:, text_color:, size:, fill:, options:, width:)
    @options = options.each_with_index.map do |option, index|
      Option.new(name: name, position: index + 1, text: option, size: size, text_color: text_color, fill: fill, width: width)
    end
    @menu = Menu.new(name: name, color: text_color, example: @options[0], size: options.size)
  end

  def frames
    options + [menu]
  end

  class Option < Shape
    STROKE_WIDTH = 0.25
    CORNER_RADIUS = STROKE_WIDTH * 2
    attr_reader :slug

    def initialize(name:, position:, text:, text_color:, fill:, size:, width:)
      @slug = Pathname("#{name}-#{position}")

      label = Label.new(color: text_color, alignment: :center, size: size, text: text, width: width)
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

  class Menu < Shape
    CORNER_RADIUS = 1.0
    attr_reader :slug

    def initialize(name:, color:, example:, size:)
      @slug = Pathname("#{name}-menu")
      right = (example.width + PADDING) / 2.0
      bottom = (example.height * size + PADDING) / 2.0
      super(top: -bottom, right: right, bottom: bottom, left: -right)

      box = Box.new(top: top, right: right, bottom: bottom, left: left,
                    fill: color, corner_radius: CORNER_RADIUS,
                    stroke: :none, stroke_width: 0)


      @shapes = [box]
    end

    def draw(canvas)
      @shapes.each { |shape| shape.draw(canvas) }
    end
  end
end
