MM_PER_INCH = 25.4
PX_PER_INCH = 75.0
PX_PER_MM = PX_PER_INCH / MM_PER_INCH
MM_PER_HP = 5.08

STROKE_WIDTH = 0.35
STROKE_INSET = STROKE_WIDTH / 2.0
PADDING = 1.0

PLUGIN_LABEL_INSET = 9.0
PANEL_HEIGHT = 128.5

PLUGIN_FONT = 12.0 / PX_PER_MM
LARGE_FONT = 9.0 / PX_PER_MM
SMALL_FONT = 7.0 / PX_PER_MM

class Text
  ASCENT_RATIO = 2.0 / 3.0 # For Proxima Nova font

  def initialize(text:, size:, color:)
    @text = text
    @size = size
    @color = color
  end

  def ascent
    @size * ASCENT_RATIO
  end

  def descent
    @size - ascent
  end

  def svg(x:, y:, attributes:)
    %Q[<text #{attributes} fill="#{@color}" x="#{x}" y="#{y}" style="font-family:Proxima Nova;font-weight:bold;font-size:#{@size}px">#{@text}</text>]
  end
end

class Bounded
  attr_reader :top, :right, :bottom, :left

  def initialize(top:, right:, bottom:, left:)
    @top = top
    @right = right
    @bottom = bottom
    @left = left
  end

  def width
    @right - @left
  end

  def height
    @bottom - @top
  end

  def center
    OpenStruct.new(x: width / 2.0 + left, y: height / 2.0 + top)
  end

  def translate(delta_x: 0, delta_y: 0)
    @top += delta_y
    @right += delta_x
    @bottom += delta_y
    @left += delta_x
  end

  def move_center_to(x: center.x, y: center.y)
    delta_x = x - center.x
    delta_y = y - center.y
    translate(delta_x: delta_x, delta_y: delta_y)
  end
end

class Label < Bounded
  ALIGNMENT_ATTRIBUTES_TEMPLATE = %[dominant-baseline="%s" text-anchor="%s"]
  ALIGNMENT_ATTRIBUTES = {
      above: ALIGNMENT_ATTRIBUTES_TEMPLATE % %w{baseline middle},
      below: ALIGNMENT_ATTRIBUTES_TEMPLATE % %w{hanging middle},
      right_of: ALIGNMENT_ATTRIBUTES_TEMPLATE % %w{middle start},
  }

  def initialize(text, padding, alignment, control)
    @text = text
    @alignment = alignment
    case alignment
    when :above
      @x = control.center.x
      @y = control.top - padding
    when :below
      @x = control.center.x
      @y = control.bottom + padding
    when :right_of
      @x = control.right + padding
      @y = control.center.y
    else
      @x = control.center.x
      @y = control.center.y
    end
    super(top: @y - @text.ascent, right: @x, bottom: @y + @text.descent, left: @x)
  end

  def svg
    @text.svg(x: @x, y: @y, attributes: ALIGNMENT_ATTRIBUTES[@alignment])
  end
end


class Box < Bounded
  CORNER_RADIUS = 1.0
  BUFFER = PADDING + STROKE_INSET

  def initialize(content_bounds:, style:, foreground:, background:)
    super(top: content_bounds.top - BUFFER, right: content_bounds.right + BUFFER, bottom: content_bounds.bottom + BUFFER, left: content_bounds.left - BUFFER)
    @stroke = foreground
    @fill = style == :reverse ? foreground : background
  end

  def svg
    %Q[
      <rect x="#{left}" y="#{top}" width="#{width}" height="#{height}"
        stroke-width="#{STROKE_WIDTH}" rx="#{CORNER_RADIUS}" ry="#{CORNER_RADIUS}"
        stroke="#{@stroke}" fill="#{@fill}"/>
      ]
  end

  def self.around(content:, style:, foreground:, background:)
    content_bounds = Bounded.new(top: content.map(&:top).min, right: content.map(&:right).max, bottom: content.map(&:bottom).max, left: content.map(&:left).min)
    Box.new(content_bounds: content_bounds, style: style, foreground: foreground, background: background)
  end
end

class Control < Bounded
  def initialize(x:, y:, width:, height: width)
    super(top: y - height / 2.0, right: x + width / 2.0, bottom: y + height / 2.0, left: x - width / 2.0)
  end
end

class RoundControl < Control
  attr_reader :diameter

  def initialize(x:, y:, diameter:)
    super(x: x, y: y, width: diameter)
    @diameter = diameter
  end

  def radius
    diameter / 2.0
  end
end

class ButtonControl < RoundControl
  attr_reader :name
  DIAMETER = 6.0

  def initialize(x: 0.0, y: 0.0, style: :normal, state: 1, foreground:, background:)
    super(x: x, y: y, diameter: DIAMETER)
    @name = "button-#{style}-#{state}"
    foreground, background = background, foreground if style == :reverse

    @stroke = foreground
    @fill = state == 2 ? background : foreground
  end

  def align(padding, alignment, other)
    new_x = case alignment
            when :right_of
              other.right + padding + radius
            when :left_of
              other.left - padding - radius
            else
              center.x
            end
    move_center_to(x: new_x, y: other.center.y)
  end

  def svg
    stroke_width = diameter / 6.0
    circle_diameter = diameter - stroke_width
    circle_radius = circle_diameter / 2.0
    %Q[
      <circle cx="#{center.x}" cy="#{center.y}" r="#{circle_radius}" stroke-width="#{stroke_width}" fill="#{@fill}" stroke="#{@stroke}"/>
    ]
  end
end

class ToggleButtonControl < Bounded
  attr_reader :name

  def initialize(name:, position:, button:, label:)
    super(top: button.top - 3.0 - SMALL_FONT, right: button.right, bottom: button.bottom, left: button.left)
    @label = label
    @button = button
    @name = "button-#{name}-#{position}"
  end

  def svg
    [@button.svg, @label.svg].join
  end
end

class KnobControl < RoundControl
  attr_reader :name

  DIAMETERS = {
      huge: 19.0,
      large: 12.7,
      medium: 10.0,
      small: 8.4,
      tiny: 7.0,
  }

  def initialize(x: 0.0, y: 0.0, size: :large, foreground:, background:)
    super(x: x, y: y, diameter: DIAMETERS[size])
    @name = "knob-#{size}"
    @knob_color = foreground
    @pointer_color = background
  end

  def svg
    pointer_width = radius / 8.0
    pointer_length = radius - pointer_width
    %Q[
      <g transform="translate(#{center.x} #{center.y})" stroke="#{@pointer_color}" fill="#{@knob_color }">
        <circle r="#{radius}" stroke="none"/>
        <line y2="-#{pointer_length}" stroke-width="#{pointer_width }" stroke-linecap="round"/>
      </g>
    ]
  end
end

class PortControl < RoundControl
  DIAMETER = 8.4

  def initialize(x: 0.0, y: 0.0, foreground:, background:)
    super(x: x, y: y, diameter: 8.4)
    @foreground = foreground
    @background = background
  end

  def name
    'port'
  end

  def svg
    stroke_width = diameter * 0.025
    sleeve_diameter = diameter - stroke_width
    step = sleeve_diameter / 7.0
    sleeve_radius = sleeve_diameter / 2.0
    ring_radius = sleeve_radius - step
    tip_radius = ring_radius - step
    %Q[
    <g transform="translate(#{center.x} #{center.y})" stroke="#{@foreground}" fill="#{@background}" stroke-width="#{stroke_width}">
      <circle r="#{sleeve_radius}"/>
      <circle r="#{ring_radius }"/>
      <circle r="#{tip_radius}" fill="#{@foreground}"/>
    </g>
    ]
  end
end

class SwitchControl < Control
  attr_reader :name

  WIDTH = 3.0

  def initialize(x: 0.0, y: 0.0, size:, selection:, foreground:, background:)
    super(x: x, y: y, width: WIDTH, height: (size) * WIDTH)
    @name = "thumb-#{size}-#{selection}"

    @size = size
    @foreground = foreground
    @background = background
    @position =
        case selection
        when size
          1.0
        when 1
          -1.0
        else
          0.0
        end
  end

  def svg
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
    lever_offset = lever_distance * -@position
    lever = (-2..2)
                .map {|index| knurl_spacing * index + lever_offset}
                .map {|y| %Q[<line x1="#{knurl_left}" x2="#{knurl_right}" y1="#{y}" y2="#{y}" stroke-width="#{knurl_stroke_width}" stroke-linecap="round"/>]}
                .join("\n")
    %Q[
      <g transform="translate(#{center.x} #{center.y})" fill="#{@background}" stroke="#{@foreground}">
        <rect x="#{box_left}" y="#{box_top}" width="#{box_width}" height="#{box_height}"
              rx="#{corner_radius}" ry="#{corner_radius}"
              stroke-width="#{box_stroke_width}"/>
        #{lever}
      </g>
    ]
  end
end
