require 'builder'
require 'pathname'

class SvgFile
  attr_reader :path

  def initialize(path:, content:)
    @path = Pathname(path)
    @content = content
  end

  def to_svg
    @svg ||= draw(Builder::XmlMarkup.new(indent: 2))
  end

  def write
    @path.open('w') { |file| file.write(to_svg) }
  end

  def draw(canvas)
    width = @content.width
    height = @content.height
    dx = @content.left
    dy = @content.top
    canvas.svg(version: '1.1', xmlns: 'http://www.w3.org/2000/svg',
               width: "#{width}mm", height: "#{height}mm",
               viewBox: "#{dx} #{dy} #{width} #{height}") do |svg|
      @content.draw(svg)
    end
  end
end
