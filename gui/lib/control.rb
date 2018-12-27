module DHE
  class Control
    attr_reader :name, :row, :column, :x_offset, :y_offset

    def initialize(module_, options)
      @module = module_
      @name = options[:name]
      @row = options[:row]
      @column = options[:column]
      @x_offset = options[:x_offset] || 0
      @y_offset = options[:y_offset] || 0
    end

    def draw_manual(svg:, x:, y:)
      draw_faceplate(svg: svg, x: x, y: y)
      draw_hardware(svg: svg, x: x, y: y)
    end
  end
end