require_relative 'control'

module DHE
  class Button < RoundControl
    DIAMETER = 6.0

    def initialize(spec:, foreground:, background:)
      super(spec: spec, diameter: DIAMETER)
      @label = spec[:label]
      @style = spec[:style]
    end

    def draw_on_image(panel:, svg:)
      draw_inner_svg(panel: panel, svg: svg)
    end

    def draw_on_panel(panel:, svg:)
    end

    def draw_inner_svg(panel:, svg:)
      stroke_width = diameter / 6.0
      circle_diameter = diameter - stroke_width
      circle_radius = circle_diameter / 2.0
      x = panel.x(column)
      y = panel.y(row)
      fill = panel.foreground
      stroke = panel.foreground
      svg.circle(cx: x, cy: y, r: circle_radius, 'stroke-width' => stroke_width, fill: fill, stroke: stroke)
    end
  end
end