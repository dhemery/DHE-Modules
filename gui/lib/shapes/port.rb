require_relative 'shape'

class Port < RoundShape
  DIAMETER = 8.4

  def initialize(foreground:, background:)
    super(DIAMETER)
    @foreground = foreground
    @background = background
  end

  def draw(canvas)
    stroke_width = diameter * 0.025
    sleeve_diameter = diameter - stroke_width
    step = sleeve_diameter / 7.0
    sleeve_radius = sleeve_diameter / 2.0
    ring_radius = sleeve_radius - step
    tip_radius = ring_radius - step
    canvas.g(stroke: @foreground, fill: @background, 'stroke-width' => stroke_width) do |g|
      g.circle(r: sleeve_radius)
      g.circle(r: ring_radius)
      g.circle(r: tip_radius, fill: @foreground)
    end
  end

  def slug
    'port'
  end
end
