require_relative 'shape'
require_relative 'label'
require_relative '../dimensions'

class PicklistOption < BoundedShape
  attr_reader :slug

  def initialize(name:, text:, position:, color:, width:)
    right = width / 2.0
    left = -right
    @label = Label.new(color: color, alignment: :above, size: :small, text: text, underline: true)
    @slug = Pathname("picklist-#{name}-#{position}")
    super(top: @label.top, right: right, bottom: -@label.top, left: left)
  end

  def draw(canvas)
    @label.draw(canvas)
  end

  def has_text?
    true
  end
end
