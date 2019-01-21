require_relative '../control'
require_relative '../dimensions'
require_relative '../shapes/label'
require_relative 'button'

class StepperState
  attr_reader :slug, :control, :label, :button

  def initialize(control:, slug:, button:, label:)
    @slug = slug
    @button = button
    @label = label
    @label_offset = button.radius + PADDING
  end

  def draw(svg:, x:, y:)
    @label.draw(svg: svg, x: x, y: y - @label_offset)
    @button.default_state.draw(svg: svg, x: x, y: y)
  end
end

class Stepper < Control
  def initialize(x:, y:, name:, labels:, foreground:, background:, enabled:, selection:)
    @draws_on_faceplate = enabled
    @has_text = true
    button = Button.new(x: x, y: y, pressed_color: background, ring_color: foreground, style: :normal)
    label_offset = button.radius + PADDING
    @states = labels.each_with_index.map do |label, index|
      StepperState.new(
          control: self,
          slug: "stepper-#{name}-#{index + 1}",
          label: Label.new(x: x, y: y - label_offset,
                           color: foreground, text: label, size: :small),
          button: button
      )
    end
    @default_state = @states[selection - 1]
    @bottom = y + (button.y - @default_state.label.top)

    super(x: x, y: y,
          top: @default_state.label.top, right: button.right, bottom: bottom, left: button.left)
  end
end
