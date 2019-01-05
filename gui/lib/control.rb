require_relative 'control'

module DHE
  class Control
    attr_reader :faceplate, :width, :height, :top, :right, :bottom, :left, :x, :y

    def initialize(faceplate:, top:, right:, bottom:, left:, x: (right + left) / 2.0, y: (bottom + top) / 2.0)
      @faceplate = faceplate
      @top = top
      @right = right
      @bottom = bottom
      @left = left
      @width = right - left
      @height = bottom - top
      @x = x
      @y = y
    end

    def self.centered(x:, y:, width:, height: width)
      {left: x - width / 2, right: x + width / 2, top: y - height / 2, bottom: y + height / 2, }
    end
  end

  class RoundControl < Control
    attr_reader :diameter

    def initialize(faceplate:, x:, y:, diameter:)
      super(faceplate: faceplate, **Control::centered(x: x, y: y, width: diameter, height: diameter))
      @diameter = diameter
    end

    def radius
      diameter / 2
    end
  end
end
