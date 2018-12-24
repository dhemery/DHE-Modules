require_relative 'control'

module DHE
  class Port < RoundControl
    DIAMETER = 8.4

    def initialize(spec:)
      super(spec: spec, diameter: DIAMETER)
      @label = spec[:label]
      @style = spec[:style]
      @special = spec[:special]
    end

    def draw_foreground_svg(svg:, x:, y:, foreground:, background:)
      stroke_width = DIAMETER * 0.025
      sleeve_diameter = DIAMETER - stroke_width
      step = sleeve_diameter / 7.0
      sleeve_radius = sleeve_diameter / 2.0
      ring_radius = sleeve_radius - step
      tip_radius = ring_radius - step
      svg.g(transform: "translate(#{x} #{y})", stroke: foreground, fill: background, 'stroke-width' => stroke_width) do |g|
        g.circle(r: sleeve_radius)
        g.circle(r: ring_radius)
        g.circle(r: tip_radius, fill: foreground)
      end
    end
  end
end