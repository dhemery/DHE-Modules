require_relative 'control'

module DHE
  class Button < RoundControl
    DIAMETER = 6.0

    def initialize(spec:)
      super(spec: spec, diameter: DIAMETER)
      @label = spec[:label]
      @style = spec[:style]
    end

    def draw_foreground_svg(x:, y:, svg:, foreground:, background:)
      ring_color = foreground
      center_color = foreground
      stroke_width = DIAMETER / 6.0
      circle_diameter = DIAMETER - stroke_width
      circle_radius = circle_diameter / 2.0
      svg.circle(cx: x, cy: y, r: circle_radius, 'stroke-width' => stroke_width, fill: center_color, stroke: ring_color)
    end
  end
end