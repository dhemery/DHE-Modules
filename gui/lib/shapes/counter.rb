require_relative 'dimensions'
require_relative 'shape'

module DHE
  class Counter < Shape
    def initialize(faceplate:, x:, y:, name:, labels:, enabled:, position:)
      @slug = "counter-#{@name}"
      @button = Button.new(faceplate: faceplate, x: x, y: y)
      @labels = labels.map { |label| Label.new(faceplate: faceplate, x: x, y: @button.top - PADDING, text: label, size:
          :small) }
      super(faceplate: faceplate, x: x, y: y, top: @labels[0].top, right: @button.right, bottom: @button.bottom, left:
          @button.left)
      @enabled = enabled
      @selection = position
    end

    def draw(svg:, x: @x, y: @y)
      return unless @enabled
      @labels[@selection - 1].draw(svg: svg, x: x, y: @button.top - PADDING)
      @button.draw(svg: svg, x: x, y: y)
    end

    def path(position)
      faceplate.slug / "#{@slug}-#{position}"
    end

    def control_file(label:, position:)
      path = path(position)
      width = @button.width
      height = @button.bottom - @labels[0].top
      SvgFile.new(path: path, width: width, height: height, has_text: true) do |svg|
        @button.draw(svg: svg, x: 0.0, y: 0.0)
        label.draw(svg: svg, x: 0.0, y: -(@button.top - PADDING))
      end
    end

    def control_files
      @labels.each_with_index.map { |label, index| control_file(label: label, position: index + 1) }
    end
  end
end
