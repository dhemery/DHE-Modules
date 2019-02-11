require_relative 'shape'
require_relative 'button'
require_relative '../dimensions'

class Stepper < BoundedShape
  attr_reader :slug

  def initialize(color:, name:, text:, position:)
    @button = Button.new(foreground: color, background: color, state: :released)
    @label = Label.new(color: color, alignment: :above, size: :large, text: text)
                 .translate(0.0, @button.top - PADDING)
    @slug = "stepper-#{name}-#{position}"
    super(top: @label.top, right: @button.right, bottom: @button.bottom, left: @button.left)
  end

  def draw(canvas)
    @label.draw(canvas)
    @button.draw(canvas)
  end
end
