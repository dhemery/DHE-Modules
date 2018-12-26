module DHE
  class Control
    attr_reader :name, :row, :column

    def initialize(module_, options)
      @module = module_
      @name = options[:name]
      @row = options[:row]
      @column = options[:column]
    end

    def draw_manual(svg:, x:, y:)
      draw_faceplate(svg: svg, x: x, y: y)
      draw_hardware(svg: svg, x: x, y: y)
    end
  end
end