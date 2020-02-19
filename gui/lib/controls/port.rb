require_relative '../shapes/circle'

class Port < RoundShape
  DIAMETER = 8.4
  STROKE_WIDTH = DIAMETER * 0.025
  SLEEVE_DIAMETER = DIAMETER - STROKE_WIDTH
  SLEEVE_RADIUS = SLEEVE_DIAMETER / 2
  REDUCTION = SLEEVE_DIAMETER / 7
  RING_RADIUS = SLEEVE_RADIUS - REDUCTION
  TIP_RADIUS = RING_RADIUS - REDUCTION

  attr_reader :slug

  def initialize(foreground:, background:)
    super(DIAMETER)
    @slug = Pathname('port')
    @shapes = [
      Circle.new(radius: SLEEVE_RADIUS, fill: background, stroke: foreground, stroke_width: STROKE_WIDTH),
      Circle.new(radius: RING_RADIUS, fill: background, stroke: foreground, stroke_width: STROKE_WIDTH),
      Circle.new(radius: TIP_RADIUS, fill: foreground, stroke: foreground, stroke_width: STROKE_WIDTH),
    ]
  end

  def draw(canvas)
    @shapes.each { |shape| shape.draw(canvas) }
  end
end
