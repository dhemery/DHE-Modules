require_relative 'control'

module DHE
  class Port < Control
    DIAMETER = 8.4

    def initialize(spec:, foreground:, background:)
      super(spec: spec)
      @label = spec[:label]
      @style = spec[:style]
      @special = spec[:special]
    end

    def to_s
      "#{super} #{@style} port #{@special}"
    end

    def draw_on_image(panel:, svg:)
      x = panel.x(column)
      y = panel.y(row)
      draw_inner_svg(panel: panel, x: x, y: y, svg: svg)
    end

    def draw_inner_svg(panel:, x:, y:, svg:)
      stroke_width = DIAMETER * 0.025
      sleeve_diameter = DIAMETER - stroke_width
      step = sleeve_diameter / 7.0
      sleeve_radius = sleeve_diameter / 2.0
      ring_radius = sleeve_radius - step
      tip_radius = ring_radius - step
      background = panel.background
      foreground = panel.foreground
      svg.g(transform: "translate(#{x} #{y})", stroke: foreground, fill: background, 'stroke-width' => stroke_width) do |g|
        g.circle(r: sleeve_radius)
        g.circle(r: ring_radius)
        g.circle(r: tip_radius, fill: foreground)
      end
    end
  end
end