require_relative 'shape'
require_relative 'button'
require_relative '../dimensions'

class Stepper < BoundedShape
  MARKER_RADIUS = Button::DIAMETER / 8
  MARKER_STROKE_WIDTH = STROKE_WIDTH * 1.4
  attr_reader :slug

  def initialize(name:, text:, position:, foreground:, background:, width: Button::DIAMETER)
    right = [Button::DIAMETER, width].max / 2
    left = -right
    @button = Button.new(foreground: foreground, background: foreground, state: :released)
    @label = Label.new(color: foreground, alignment: :above, size: :small, text: text)
                 .translate(0.0, @button.top - PADDING)
    @slug = Pathname("stepper-#{name}-#{position}")
    @marker_color = background
    super(top: @label.top, right: right, bottom: -@label.top, left: left)
  end

  def draw(canvas)
    @label.draw(canvas)
    @button.draw(canvas)
    canvas.line(x1: -MARKER_RADIUS, y1: 0, x2: MARKER_RADIUS, y2: 0,
                'stroke-width' => MARKER_STROKE_WIDTH, stroke: @marker_color, 'stroke-linecap' => 'round')
    canvas.line(x1: 0, y1: -MARKER_RADIUS, x2: 0, y2: MARKER_RADIUS,
                'stroke-width' => MARKER_STROKE_WIDTH, stroke: @marker_color, 'stroke-linecap' => 'round')
  end

  def has_text?
    true
  end
end
