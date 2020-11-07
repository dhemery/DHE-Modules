require 'builder'
require 'pathname'

class SvgFile
  attr_reader :path

  def initialize(path:)
    @path = Pathname(path)
  end

  def to_svg
    @svg ||= draw(Builder::XmlMarkup.new(indent: 2))
  end

  def write
    @path.open('w') { |file| file.write(to_svg) }
  end
end

class ModuleSvgFile < SvgFile
  def initialize(path:, mod:)
    super(path: path)
    @faceplate = mod.faceplate_shape
    @overlay = mod.overlay_shape
  end

  def draw(canvas)
    width = @faceplate.width
    height = @faceplate.height
    canvas.svg(version: '1.1', xmlns: 'http://www.w3.org/2000/svg',
               width: "#{width}mm", height: "#{height}mm",
               viewBox: "0 0 #{width} #{height}") do |svg|
      svg.g(id: 'faceplate') do |g|
        @faceplate.draw(g)
      end
      svg.g(id: 'overlay') do |g|
        @overlay.draw(g)
      end unless @overlay.nil?
    end
  end
end

class ControlSvgFile < SvgFile
  def initialize(path:, content:)
    super(path: path)
    @content = content
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