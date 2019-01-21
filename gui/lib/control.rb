require_relative 'shape'
require_relative 'svg_file'

class Control < Shape
  attr_reader :states, :default_state

  def initialize(top:, right:, bottom:, left:, x: (right + left) / 2.0, y: (bottom + top) / 2.0)
    super(top: top, right: right, bottom: bottom, left: left, x: x, y: y)
    @draws_on_faceplate = true
    @has_text = false
  end

  def svg_files(dir)
    @states.map do |state|
      path = dir / state.slug
      svg_file(path: path, has_text: @has_text) do |svg|
        state.draw(svg: svg, x: width / 2.0, y: height / 2.0)
      end
    end
  end

  def draw_faceplate(svg:)
    @default_state.draw(svg: svg, x: @x, y: @y) if @draws_on_faceplate
  end

  def svg_file(path:, has_text: false)
    SvgFile.new(path: path,
                width: "#{width}mm", height: "#{height}mm", viewBox: "0 0 #{width} #{height}",
                has_text: has_text) do |svg|
      yield svg
    end
  end
end

class RoundControl < Control
  attr_reader :diameter

  def initialize(x:, y:, diameter:)
    super(**Control::centered(x: x, y: y, width: diameter, height: diameter))
    @diameter = diameter
  end

  def radius
    diameter / 2
  end
end
