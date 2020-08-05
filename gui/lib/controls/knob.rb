require_relative '../shapes/shape'
require_relative '../shapes/circle'
require_relative '../shapes/line'

class Knob < Shape
  DIAMETERS = { huge: 19.0, large: 12.7, medium: 10.0, small: 8.4, tiny: 7.0, eensie: 4.5, }

  attr_reader :slug

  def initialize(knob_color:, pointer_color:, size:)
    radius = DIAMETERS[size] / 2.0
    super(top: -radius, right: radius, bottom: radius, left: -radius)
    @slug = Pathname("knob-#{size}")

    pointer_width = radius / 8.0
    pointer_length = radius - pointer_width

    @shapes = [
      Circle.new(radius: radius, fill: knob_color, stroke: :none, stroke_width: 0),
      Line.new(x1: 0, y1: 0, x2: 0, y2: -pointer_length, width: pointer_width, stroke: pointer_color, cap: :round),
    ]
  end

  def draw(canvas)
    @shapes.each { |shape| shape.draw(canvas) }
  end

  def frames
    [self]
  end
end
