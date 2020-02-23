require_relative '../shapes/circle'

class Port < Shape
  DIAMETER = 8.4
  RADIUS = DIAMETER / 2.0
  SHADOW_THICKNESS = 0.45
  METAL_THICKNESS = 0.95
  SLEEVE_DIAMETER = DIAMETER - METAL_THICKNESS - SHADOW_THICKNESS
  RING_DIAMETER = SLEEVE_DIAMETER - METAL_THICKNESS - METAL_THICKNESS - SHADOW_THICKNESS

  attr_reader :slug

  def initialize(metal_color:, shadow_color:)
    super(top: -RADIUS, right: RADIUS, bottom: RADIUS, left: -RADIUS)
    @slug = Pathname('port')

    @shapes = [
      Circle.new(radius: RADIUS, fill: shadow_color, stroke: :none, stroke_width: 0,),
      Circle.new(radius: SLEEVE_DIAMETER / 2.0, stroke: metal_color, stroke_width: METAL_THICKNESS, fill: :none),
      Circle.new(radius: RING_DIAMETER / 2.0, stroke: metal_color, stroke_width: METAL_THICKNESS, fill: :none),
    ]
  end

  def draw(canvas)
    @shapes.each { |shape| shape.draw(canvas) }
  end
end
