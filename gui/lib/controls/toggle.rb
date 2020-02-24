require_relative '../shapes/shape'
require_relative '../shapes/box'
require_relative '../shapes/line'

class Toggle
  attr_reader :states

  def initialize(foreground:, background:, size:)
    @states = (0...size).map do |index|
      State.new(foreground: foreground, background: background, size: size, position: index + 1)
    end
  end

  def frames
    states
  end

  class State < Shape
    WIDTH = 3.0
    HOUSING_THICKNESS = WIDTH / 8.0
    HOUSING_INSET = HOUSING_THICKNESS / 2.0
    HOUSING_WIDTH = WIDTH - HOUSING_THICKNESS
    HOUSING_CORNER_RADIUS = HOUSING_THICKNESS / 2.0
    KNURL_SPACING = 0.5
    KNURL_THICKNESS = KNURL_SPACING / 2.0
    KNURL_RIGHT = HOUSING_WIDTH / 2.0 - KNURL_SPACING
    KNURL_LEFT = -KNURL_RIGHT
    KNURL_OFFSETS = {
      2 => [WIDTH / 2.0 - KNURL_THICKNESS, -WIDTH / 2.0 + KNURL_THICKNESS],
      3 => [WIDTH - KNURL_THICKNESS, 0.0, -WIDTH + KNURL_THICKNESS]
    }

    attr_reader :slug

    def initialize(foreground:, background:, size:, position:)
      super(top: -(size * WIDTH) / 2.0, right: WIDTH / 2.0, bottom: (size * WIDTH) / 2.0, left: -WIDTH / 2.0)
      @slug = Pathname("toggle-#{size}-#{position}")

      housing_width = width - HOUSING_THICKNESS
      housing_height = height - HOUSING_THICKNESS
      housing = Box.centered(width: housing_width, height: housing_height,
                             corner_radius: HOUSING_CORNER_RADIUS,
                             stroke_width: HOUSING_THICKNESS,
                             fill: background, stroke: foreground)

      knurl_offset = KNURL_OFFSETS[size][position - 1]
      knurls = (-2..2).map { |index| KNURL_SPACING * index }
                      .map do |knurl_y|
        Line.new(x1: KNURL_LEFT, y1: knurl_y + knurl_offset,
                 x2: KNURL_RIGHT, y2: knurl_y + knurl_offset,
                 width: KNURL_THICKNESS,
                 stroke: foreground, cap: 'round')
      end

      @shapes = [housing] + knurls
    end

    def draw(svg)
      @shapes.each do |shape|
        shape.draw(svg)
      end
    end

  end
end
