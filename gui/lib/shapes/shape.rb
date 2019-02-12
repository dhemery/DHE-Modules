class Translated < SimpleDelegator
  def initialize(dx:, dy:, shape:)
    @dx = dx
    @dy = dy
    @shape = shape
  end

  def x
    @shape.x + @dx
  end

  def y
    @shape.y + @dy
  end

  def top
    @shape.top + @dy
  end

  def right
    @shape.right + @dx
  end

  def bottom
    @shape.bottom + @dy
  end

  def left
    @shape.left + @dx
  end

  def width
    @shape.width
  end

  def height
    @shape.height
  end

  def draw(canvas)
    canvas.g(transform: "translate(#{@dx},#{@dy})") do |g|
      @shape.draw(g)
    end
  end
end

class Shape
  def x
    0.0
  end

  def y
    0.0
  end

  def translate(dx, dy)
    Translated.new(dx: dx, dy: dy, shape: self)
  end

  def has_text?
    false
  end
end

class BoundedShape < Shape
  attr_reader :top, :right, :bottom, :left, :width, :height

  def initialize(top:, right:, bottom:, left:)
    @top = top
    @right = right
    @bottom = bottom
    @left = left
    @width = right - left
    @height = bottom - top
  end
end

class CenteredShape < BoundedShape
  def initialize(width:, height:)
    right = width / 2.0
    bottom = height / 2.0
    super(right: right, bottom: bottom, left: -right, top: -bottom)
  end
end

class RoundShape < CenteredShape
  def initialize(diameter)
    super(width: diameter, height: diameter)
  end

  def diameter
    width
  end

  def radius
    right
  end
end

class CompositeShape < BoundedShape
  def initialize(shapes:)
    super(top: shapes.map(&:top).min,
          right: shapes.map(&:right).max,
          bottom: shapes.map(&:bottom).max,
          left: shapes.map(&:left).min)
    @shapes = shapes
  end

  def draw(canvas)
    @shapes.each do |shape|
      shape.draw(canvas)
    end
  end
end