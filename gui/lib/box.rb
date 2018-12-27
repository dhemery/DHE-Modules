require_relative 'dimensions'
require_relative 'shape'

module DHE
  class Box < Shape
    CORNER_RADIUS = 1.0
    BUFFER = PADDING + STROKE_INSET

    def initialize(module_:, top:, right:, bottom:, left:, style: :normal)
      super(module_: module_, top: top - BUFFER, right: right + BUFFER, bottom: bottom + BUFFER, left: left - BUFFER)
      @stroke = module_.foreground
      @fill = style == :normal ? module_.background : @stroke
    end

    def draw_svg(svg:, x:, y:)
      svg.rect(x: @left + x, y: @top + y, width: @width, height: @height, 'stroke-width' => STROKE_WIDTH, rx: CORNER_RADIUS, ry: CORNER_RADIUS, stroke: @stroke, fill: @fill)
    end
  end
end