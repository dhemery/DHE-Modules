require_relative 'control'

module DHE
  class Counter < RoundControl
    DIAMETER = 6.0

    def initialize(spec:)
      super(name: spec[:name], row: spec[:row], column: spec[:column], diameter: DIAMETER)
      @labels = spec[:labels].map{ |label| Text.new(text: label, size: :small, alignment: :above) }
      @position = (spec[:position] || 1) - 1
      special = spec[:special] || []
      @invisible = special.include? 'invisible'
    end

    def draw_foreground_svg(svg:, x:, y:, foreground:, background:)
      ring_color = foreground
      center_color = foreground
      stroke_width = DIAMETER / 6.0
      circle_diameter = DIAMETER - stroke_width
      circle_radius = circle_diameter / 2.0
      svg.circle(cx: x, cy: y, r: circle_radius, 'stroke-width' => stroke_width, fill: center_color, stroke: ring_color)
    end

    def draw_background_svg(x:, y:, svg:, foreground:, background:)
      @labels[@position].draw_svg(svg: svg, x: x, y: top(y: y), color: foreground) unless @invisible
    end
  end
end