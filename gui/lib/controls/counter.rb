require_relative '../control'
require_relative '../dimensions'

module DHE
  class Counter < Control
    def initialize(faceplate:, x:, y:, name:, labels:, enabled:, position:)
      @name = name
      @slug = "counter-#{@name}"
      @button = Button.new(faceplate: faceplate, x: x, y: y)
      @label_offset = @button.radius + PADDING
      @labels = labels.map {|label| Label.new(faceplate: faceplate, x: x, y: y - @label_offset, text: label, size:
          :small)}
      super(faceplate: faceplate, x: x, y: y, top: @labels[0].top, right: @button.right, bottom: @button.bottom, left:
          @button.left)
      @enabled = enabled
      @selection = position
    end

    def draw(svg:, x: @x, y: @y)
      return unless @enabled
      @labels[@selection - 1].draw(svg: svg, x: x, y: y - @label_offset)
      @button.draw(svg: svg, x: x, y: y)
    end

    def svg_files
      @labels.each_with_index.map {|label, index| svg_file(label: label, position: index + 1)}
    end

    def svg_file(label:, position:)
      path = faceplate.slug / "#{@slug}-#{position}"
      width = @button.width
      height = (@button.y - @labels[0].top) * 2.0
      x = width / 2.0
      y = height / 2.0
      SvgFile.new(path: path, width: "#{width}mm", height: "#{height}mm", has_text: true, viewBox: "0 0 #{width} #{height}") do |svg|
        @button.draw(svg: svg, x: x, y: y)
        label.draw(svg: svg, x: x, y: y - @label_offset)
      end
    end
  end
end
