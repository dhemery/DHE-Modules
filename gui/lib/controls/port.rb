require_relative '../control'

class PortState
  attr_reader :slug, :control

  def initialize(control:, slug:, foreground:, background:)
    @control = control
    @slug = slug
    @foreground = foreground
    @background = background
  end

  def draw(svg:, x:, y:)
    stroke_width = control.diameter * 0.025
    sleeve_diameter = control.diameter - stroke_width
    step = sleeve_diameter / 7.0
    sleeve_radius = sleeve_diameter / 2.0
    ring_radius = sleeve_radius - step
    tip_radius = ring_radius - step
    svg.g(transform: "translate(#{x} #{y})", stroke: @foreground, fill: @background, 'stroke-width' => stroke_width) do |g|
      g.circle(r: sleeve_radius)
      g.circle(r: ring_radius)
      g.circle(r: tip_radius, fill: @foreground)
    end
  end
end

class Port < RoundControl
  DIAMETER = 8.4

  def initialize(x:, y:, foreground:, background:)
    super(x: x, y: y, diameter: DIAMETER)
    @default_state = PortState.new(control: self, slug: 'port', foreground: foreground, background: background)
    @states = [@default_state]
  end

end