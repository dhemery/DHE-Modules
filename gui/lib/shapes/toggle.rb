require_relative 'shape'

module Toggle
  WIDTH = 3.0
  STROKE_WIDTH = WIDTH / 8.0

  class Housing < CenteredShape
    WIDTH = Toggle::WIDTH - STROKE_WIDTH
    INSET = STROKE_WIDTH / 2.0
    RIGHT = WIDTH / 2.0
    LEFT = -RIGHT
    def initialize(foreground:, background:, size:)
      super(width: Toggle::WIDTH, height: size * Toggle::WIDTH)
      @line_attributes = {
          fill: background,
          stroke: foreground
      }
    end

    def draw(svg)
      box_height = height - STROKE_WIDTH
      box_top = -height / 2.0 + INSET
      svg.rect(x: LEFT, y: box_top, width: WIDTH, height: box_height,
               rx: INSET, ry: INSET,
               'stroke-width' => STROKE_WIDTH, **@line_attributes)
    end
  end

  class Lever < CenteredShape
    WIDTH = Housing::WIDTH - Toggle::STROKE_WIDTH
    SPACING = 0.5
    THICKNESS = SPACING / 2.0
    RIGHT = WIDTH / 2.0 - THICKNESS
    LEFT = -RIGHT

    OFFSETS = {
        2 => [Toggle::WIDTH / 2.0 - THICKNESS, -Toggle::WIDTH / 2.0 + THICKNESS],
        3 => [Toggle::WIDTH - THICKNESS, 0.0, -Toggle::WIDTH + THICKNESS]
    }

    def initialize(foreground:, background:, size:, position:)
      super(width: WIDTH, height: WIDTH)
      @offset = OFFSETS[size][position - 1]
      @line_attributes = {
          'stroke-linecap'.to_sym => 'round',
          fill: background,
          stroke: foreground
      }
    end

    def draw(svg)
      svg.g(transform: "translate(0.000 #{@offset})") do |g|
        (-2..2).map {|index| SPACING * index}.each do |knurl_y|
          g.line(x1: LEFT, x2: RIGHT, y1: knurl_y, y2: knurl_y,
                   'stroke-width' => THICKNESS, **@line_attributes)
        end
      end
    end
  end
end