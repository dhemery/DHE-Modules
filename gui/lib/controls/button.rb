require_relative '../shapes/circle'

class Button < RoundShape
  DIAMETER = 6.0
  HOUSING_THICKNESS = DIAMETER / 6.0
  RADIUS = (DIAMETER - HOUSING_THICKNESS) / 2.0

  attr_reader :slug

  def initialize(foreground:, background:, style: :normal, state:)
    super(DIAMETER)
    foreground, background = background, foreground unless style == :normal
    @stroke = foreground
    @fill = state == :pressed ? background : foreground
    shape = Circle.new(radius: RADIUS, fill: @fill, stroke: @stroke, stroke_width: HOUSING_THICKNESS)
    @shapes = [shape]
    style_slug = style == :reversed ? '-reversed' : ''
    state_slug = state == :pressed ? '-2' : '-1'
    @slug = Pathname("button#{style_slug}#{state_slug}")
  end

  def draw(canvas)
    @shapes.each { |shape| shape.draw(canvas) }
  end
end
