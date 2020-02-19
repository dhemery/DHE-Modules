require_relative '../shapes/circle'
require_relative '../shapes/line'

class Knob < RoundShape
  DIAMETERS = { huge: 19.0, large: 12.7, medium: 10.0, small: 8.4, tiny: 7.0, }

  attr_reader :slug

  def initialize(size:, foreground:, background:)
    super(DIAMETERS[size])
    @slug = Pathname("knob-#{size}")

    @shapes = []
    @shapes << Circle.new(radius: DIAMETERS[size] / 2.0, fill: foreground)

    pointer_width = radius / 8.0
    pointer_length = radius - pointer_width
    @shapes << Line.new(y2: -pointer_length, width: pointer_width, stroke: background, cap: 'round')
  end

  def draw(canvas)
    @shapes.each { |shape| shape.draw(canvas) }
  end
end
