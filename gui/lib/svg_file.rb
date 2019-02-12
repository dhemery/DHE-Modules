require 'builder'
require 'pathname'

class SvgFile
  attr_reader :path

  def initialize(path:, has_text:)
    @path = Pathname(path).sub_ext('.svg')
    @has_text = has_text
  end

  def to_svg
    @svg ||= draw(Builder::XmlMarkup.new(indent: 2))
  end

  def has_text?
    @has_text
  end

  def write
    @path.open('w') {|file| file.write(to_svg)}
  end
end

class ModuleSvgFile < SvgFile
  def initialize(path:, content:)
    super(path: path, has_text: true)
    @content = content
  end

  def draw(canvas)
    canvas.svg(version: '1.1', xmlns: 'http://www.w3.org/2000/svg',
               width: @content.width * PX_PER_MM, height: @content.height * PX_PER_MM) do |svg|
      svg.g(transform: "scale(#{PX_PER_MM})") do |g|
        @content.draw(g)
      end
    end
  end
end

class ControlSvgFile < SvgFile
  def initialize(path:, content:)
    super(path: path, has_text: content.has_text?)
    @content = content
  end

  def draw(canvas)
    width = @content.width
    height = @content.height
    dx = -@content.left
    dy = -@content.top
    canvas.svg(version: '1.1', xmlns: 'http://www.w3.org/2000/svg',
               width: "#{width}mm", height: "#{height}mm",
               viewBox: "0 0 #{width} #{height}") do |svg|
      svg.g(transform: "translate(#{dx} #{dy})") do |g|
        @content.draw(g)
      end
    end
  end
end