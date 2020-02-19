require_relative 'circle'

class Light < Circle
  RADIUS = 1.088
  DIAMETER = RADIUS * 2

  def initialize(color:)
    super(radius: RADIUS, fill: color)
  end
end