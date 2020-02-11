require_relative 'shape'
require_relative 'label'
require_relative '../dimensions'

class PicklistOption < BoundedShape
  BOX_CORNER_RADIUS = 0.5
  attr_reader :slug

  def initialize(name:, text:, position:, foreground:, background:, width:)
    right = width / 2.0
    left = -right
    @background = background
    @slug = Pathname("picklist-#{name}-#{position}")
    @label = Label.new(color: foreground, alignment: :center, size: :small, text: text)
    super(top: @label.top - PADDING, right: right, bottom: PADDING - @label.top, left: left)
  end

  def draw(canvas)
    canvas.rect(x: left, width: width, y: top, height: height,
                rx: BOX_CORNER_RADIUS, ry: BOX_CORNER_RADIUS,
                stroke: 'none', fill: @background)
    @label.draw(canvas)
  end

  def has_text?
    true
  end
end
