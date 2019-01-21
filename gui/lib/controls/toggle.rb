require_relative '../control'

class ToggleState
  attr_reader :slug, :control

  def initialize(control:, slug:, foreground:, background:, size:, position:)
    @control = control
    @slug = slug
    @foreground = foreground
    @background = background
    @size = size
    @position = position
  end

  def draw(svg:, x:, y:)
    width = control.width
    height = control.height
    position = case @position
                 when @size
                   1.0
                 when 1
                   -1.0
                 else
                   0.0
               end
    box_stroke_width = width / 8.0
    interior_inset = box_stroke_width / 2.0

    box_width = width - box_stroke_width
    box_height = height - box_stroke_width
    box_left = -width / 2.0 + interior_inset
    box_top = -height / 2.0 + interior_inset

    interior_width = box_width - box_stroke_width
    interior_height = box_height - box_stroke_width
    corner_radius = interior_inset

    knurl_stroke_width = 0.25
    knurl_inset = knurl_stroke_width * 2.0
    knurl_length = interior_width - knurl_inset
    knurl_left = knurl_length / -2.0
    knurl_right = knurl_left + knurl_length
    knurl_spacing = knurl_stroke_width * 2.0

    lever_height = knurl_spacing * 4.0 + knurl_stroke_width
    lever_inset = knurl_stroke_width
    lever_distance = (interior_height - lever_height) / 2.0 - lever_inset
    lever_offset = lever_distance * -position

    svg.g(transform: "translate(#{x} #{y})", fill: @background, stroke: @foreground) do |g|
      g.rect(x: box_left, y: box_top, width: box_width, height: box_height, rx: corner_radius, ry: corner_radius, 'stroke-width' => box_stroke_width)
      (-2..2).map { |index| knurl_spacing * index + lever_offset }.each do |knurl_y|
        g.line(x1: knurl_left, x2: knurl_right, y1: knurl_y, y2: knurl_y, 'stroke-width' => knurl_stroke_width, 'stroke-linecap' => 'round')
      end
    end
  end
end

class Toggle < Control
  WIDTH = 3.0

  attr_reader :size

  def initialize(x:, y:, foreground:, background:, size:, selection:)
    super(**Control::centered(x: x, y: y, width: WIDTH, height: WIDTH * size))
    @selection = selection
    @states = (1..size).map do |position|
      ToggleState.new(
          control: self,
          slug: "toggle-#{size}-#{position}",
          foreground: foreground,
          background: background,
          size: size,
          position: position,
      )
    end
    @default_state = @states[selection - 1]
  end
end