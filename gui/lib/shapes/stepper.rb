require_relative 'shape'
require_relative 'button'
require_relative '../dimensions'

class Stepper < BoundedShape
  attr_reader :slug

  def initialize(color:, name:, text:, position:)
    @button = Button.new(foreground: color, background: color, state: :released)
    @label = Label.new(color: color, alignment: :above, size: :small, text: text)
                 .translate(0.0, @button.top - PADDING)
    @slug = Pathname("stepper-#{name}-#{position}")
    super(top: @label.top, right: @button.right, bottom: -@label.top, left: @button.left)
  end

  def draw(canvas)
    @label.draw(canvas)
    @button.draw(canvas)
  end

  def has_text?
    true
  end
end
