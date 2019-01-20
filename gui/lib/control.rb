require_relative 'shape'
require_relative 'svg_file'

class Control < Shape
  attr_reader :slug, :states, :default_state

  def initialize(slug:, top:, right:, bottom:, left:, x: (right + left) / 2.0, y: (bottom + top) / 2.0)
    super(top: top, right: right, bottom: bottom, left: left, x: x, y: y)
    @slug = slug
    @default_state = {
        slug: slug,
    }
    @states = [@default_state]
    @draws_on_faceplate = true
    @has_text = false
  end

  def svg_files(dir)
    @states.map do |state|
      path = dir / state[:slug]
      svg_file(path: path, has_text: @has_text) do |svg|
        draw_control(svg: svg, **state)
      end
    end
  end

  def draw_faceplate(svg:)
    draw(svg: svg, x: @x, y: @y, **@default_state) if @draws_on_faceplate
  end

  def draw_control(svg:, **options)
    draw(svg: svg, x: width / 2.0, y: height / 2.0, **options)
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

  def initialize(slug:, x:, y:, diameter:)
    super(slug: slug, **Control::centered(x: x, y: y, width: diameter, height: diameter))
    @diameter = diameter
  end

  def radius
    diameter / 2
  end
end
