require_relative '../control'
require_relative '../dimensions'
require_relative '../shapes/label'
require_relative 'button'

class Counter < Control
  def initialize(x:, y:, name:, labels:, foreground:, background:, enabled:, selection:)
    @name = name
    @slug = "counter-#{@name}"
    @button = Button.new(x: x, y: y, pressed_color: background, ring_color: foreground, style: :normal)
    @label_offset = @button.radius + PADDING
    @labels = labels.map do |label|
      Label.new(x: x, y: y - @label_offset,
                color: foreground, text: label, size: :small)
    end
    bottom = y + (@button.y - @labels[0].top)
    super(x: x, y: y,
          top: @labels[0].top, right: @button.right, bottom: bottom, left: @button.left)
    @enabled = enabled
    @selection = selection
  end

  def draw(svg:, x:, y:, selection: @selection)
    @labels[selection - 1].draw(svg: svg, x: x, y: y - @label_offset)
    @button.draw(svg: svg, x: x, y: y, **@button.states[0])
  end

  def draw_faceplate(svg:)
    return unless @enabled
    draw(svg: svg, x: @x, y: @y)
  end

  def svg_files(dir)
    (0...@labels.size).map do |index|
      selection = index + 1
      path = dir / "#{@slug}-#{selection}"
      svg_file(path: path, has_text: true) do |svg|
        draw_control(svg: svg, selection: selection)
      end
    end
  end
end
