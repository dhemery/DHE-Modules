require_relative 'shape'

class Toggle < CenteredShape
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
    super(width: WIDTH, height: size * WIDTH)
    @foreground = foreground
    @background = background
    @knurl_offset = KNURL_OFFSETS[size][position - 1]
    @slug = Pathname("toggle-#{size}-#{position}")
  end

  def draw(svg)
    housing_left = left + HOUSING_INSET
    housing_top = top + HOUSING_INSET
    housing_height = height - HOUSING_THICKNESS

    svg.rect(x: housing_left, y: housing_top, width: HOUSING_WIDTH, height: housing_height,
             rx: HOUSING_CORNER_RADIUS, ry: HOUSING_CORNER_RADIUS,
             'stroke-width' => HOUSING_THICKNESS,
             fill: @background, stroke: @foreground)

    svg.g(transform: "translate(0.000 #{@knurl_offset})") do |g|
      (-2..2).map {|index| KNURL_SPACING * index}.each do |knurl_y|
        g.line(x1: KNURL_LEFT, x2: KNURL_RIGHT, y1: knurl_y, y2: knurl_y,
               'stroke-width' => KNURL_THICKNESS,
               'stroke-linecap'.to_sym => 'round',
               fill: @background, stroke: @foreground)
      end
    end
  end
end
