require_relative 'control'
require_relative '../shapes/box'
require_relative '../shapes/button'
require_relative '../shapes/dimensions'
require_relative '../shapes/label'
require_relative '../shapes/port'

module DHE
  class PortControl < Control
    def initialize(module_, options)
      super(module_, options)
      style = options[:style]
      label_text = options[:label] || style
      is_input = style == 'in'
      is_output = style == 'out'
      is_boxed = is_input || is_output
      drawing_style = is_output ? :reversed : :normal
      @label = Label.new(module_: module_, text: label_text, size: :small, style: drawing_style)
      @port = Port.new(module_: module_)
      box_top = @label.top(@port.top(0))
      box_bottom = @port.bottom(0)
      box_left = @port.left(0)
      box_right = @port.right(0)
      button_position = options.fetch(:button, :none).to_sym
      if button_position != :none
        @button = Button.new(module_: module_, style: drawing_style)
        if button_position == :right
          @button_offset = @port.right(0) + PADDING + @button.radius
          box_right = @button.right(@button_offset)
        else
          @button_offset = @port.left(0) - PADDING - @button.radius
          box_left = @button.left(@button_offset)
        end
      end
      @box = Box.new(module_: module_, top: box_top, right: box_right, bottom: box_bottom, left: box_left, style: drawing_style) if is_boxed
    end

    def draw_faceplate(x:, y:, svg:)
      @box.draw_svg(svg: svg, x: x, y: y) if @box
      @label.draw_svg(svg: svg, x: x, y: @port.top(y - PADDING))
    end

    def draw_hardware(svg:, x:, y:)
      @port.draw_svg(svg: svg, x: x, y: y)
      @button.draw_svg(svg: svg, x: x + @button_offset, y: y) if @button
    end

    def control_files
      files = [@port.svg_file]
      files += [:on, :off].map { |state| @button.svg_file(state: state) } if @button
      files
    end
  end
end