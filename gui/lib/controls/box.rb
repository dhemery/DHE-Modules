require_relative '../dimensions'
require_relative '../control'

module DHE
  class Box < Control
    CORNER_RADIUS = 1.0
    BUFFER = PADDING + STROKE_INSET

    def initialize(faceplate:, top:, right:, bottom:, left:, style: :normal)
      super(faceplate: faceplate, top: top - BUFFER, right: right + BUFFER, bottom: bottom + BUFFER, left: left -
          BUFFER)
      @stroke = faceplate.foreground
      @fill = style == :normal ? faceplate.background : @stroke
    end

    def draw(svg:, x: @x, y: @y)
      svg.rect(x: x - @width / 2, y: y - @height / 2, width: @width, height: @height, 'stroke-width' => STROKE_WIDTH,
               rx:
                   CORNER_RADIUS, ry:
                   CORNER_RADIUS, stroke: @stroke, fill: @fill)
    end
  end
end