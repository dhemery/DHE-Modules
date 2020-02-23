require_relative '../shapes/circle'

class Button < RoundShape
  DIAMETER = 6.0
  RING_RADIUS = DIAMETER / 2.0
  RING_THICKNESS = DIAMETER / 6.0
  BUTTON_RADIUS = RING_RADIUS - RING_THICKNESS

  attr_reader :slug

  def initialize(color:, ring:, name: 'button', state:)
    super(DIAMETER)
    @slug = Pathname("#{name}-#{state}")

    @shapes = [
      Circle.new(radius: RING_RADIUS, fill: ring),
      Circle.new(radius: BUTTON_RADIUS, fill: color)
    ]
  end

  def draw(canvas)
    @shapes.each { |shape| shape.draw(canvas) }
  end
end
