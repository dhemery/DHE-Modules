require_relative '../control'
require_relative '../dimensions'
require_relative '../shapes/label'
require_relative 'button'

class Counter < Control
  def initialize(x:, y:, name:, labels:, foreground:, background:, enabled:, selection:)
    @button = Button.new(x: x, y: y, pressed_color: background, ring_color: foreground, style: :normal)
    @label_offset = @button.radius + PADDING
    @labels = labels.map do |label|
      Label.new(x: x, y: y - @label_offset,
                color: foreground, text: label, size: :small)
    end
    bottom = y + (@button.y - @labels[0].top)
    super(slug: "counter-#{name}", x: x, y: y,
          top: @labels[0].top, right: @button.right, bottom: bottom, left: @button.left)
    @enabled = enabled
    @states = @labels.each_with_index.map do |label, index|
      {
          slug: "#{slug}-#{index + 1}",
          label: label
      }
    end
    @default_state = @states[selection - 1]
  end

  def draw(svg:, x:, y:, **state)
    state[:label].draw(svg: svg, x: x, y: y - @label_offset)
    @button.draw(svg: svg, x: x, y: y, **@button.default_state)
  end

  def draw_faceplate(svg:)
    draw(svg: svg, x: @x, y: @y, **@default_state) if @enabled
  end
end
