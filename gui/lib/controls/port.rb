require_relative '../shapes/circle'

class Port < RoundShape
  DIAMETER = 8.4
  SHADOW_THICKNESS = 0.45
  METAL_THICKNESS = 0.95
  SLEEVE_DIAMETER = DIAMETER - METAL_THICKNESS - SHADOW_THICKNESS
  RING_DIAMETER = SLEEVE_DIAMETER - METAL_THICKNESS - METAL_THICKNESS - SHADOW_THICKNESS

  attr_reader :slug

  def initialize(metal:, shadow:)
    super(DIAMETER)
    @slug = Pathname('port')

    @shapes = [
      Circle.new(radius: radius, fill: shadow),
      Circle.new(radius: SLEEVE_DIAMETER / 2.0, stroke: metal, stroke_width: METAL_THICKNESS),
      Circle.new(radius: RING_DIAMETER / 2.0, stroke: metal, stroke_width: METAL_THICKNESS),
    ]
  end

  def draw(canvas)
    @shapes.each { |shape| shape.draw(canvas) }
  end
end
