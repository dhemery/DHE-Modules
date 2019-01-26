require_relative 'shape'

module Toggle
  WIDTH = 3.0

  class Housing < CenteredShape
    def initialize(foreground:, background:, size:)
      super(width: WIDTH, height: size * WIDTH)
      @line_attributes = {
          fill: background,
          stroke: foreground
      }
    end

    def draw(svg)
      box_stroke_width = width / 8.0
      interior_inset = box_stroke_width / 2.0

      box_width = width - box_stroke_width
      box_height = height - box_stroke_width
      box_left = -width / 2.0 + interior_inset
      box_top = -height / 2.0 + interior_inset

      corner_radius = interior_inset

      svg.rect(x: box_left, y: box_top, width: box_width, height: box_height, rx: corner_radius, ry: corner_radius, 'stroke-width' => box_stroke_width, **@line_attributes)
    end
  end

  class Lever < CenteredShape
    def initialize(foreground:, background:, size:, position:)
      super(width: WIDTH, height: WIDTH)
      @position = case position
                    when size
                      1.0
                    when 1
                      -1.0
                    else
                      0.0
                  end
      @line_attributes = {
          'stroke-linecap'.to_sym => 'round',
          fill: background,
          stroke: foreground
      }
    end

    def draw(svg)
      box_stroke_width = width / 8.0

      box_width = width - box_stroke_width
      box_height = height - box_stroke_width

      interior_width = box_width - box_stroke_width
      interior_height = box_height - box_stroke_width

      knurl_stroke_width = 0.25
      knurl_inset = knurl_stroke_width * 2.0
      knurl_length = interior_width - knurl_inset
      knurl_left = knurl_length / -2.0
      knurl_right = knurl_left + knurl_length
      knurl_spacing = knurl_stroke_width * 2.0

      lever_height = knurl_spacing * 4.0 + knurl_stroke_width
      lever_inset = knurl_stroke_width
      lever_distance = (interior_height - lever_height) / 2.0 - lever_inset
      lever_offset = lever_distance * -@position

      (-2..2).map { |index| knurl_spacing * index + lever_offset }.each do |knurl_y|
        svg.line(x1: knurl_left, x2: knurl_right, y1: knurl_y, y2: knurl_y, 'stroke-width' => knurl_stroke_width, **@line_attributes)
      end
    end
  end
end