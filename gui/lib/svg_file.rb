require 'builder'

class SvgFile
  SVG_ATTRIBUTES = {
      version: "1.1",
      xmlns: "http://www.w3.org/2000/svg"
  }
  attr_reader :path

  def initialize(path:, content:)
    @path = path.sub_ext('.svg')
    @content = content
  end

  def to_svg
    @svg ||= Builder::XmlMarkup.new(indent: 2)
                 .svg(width: @content.width, height: @content.height, **SVG_ATTRIBUTES) do |svg|
      @content.draw(svg)
    end
  end

  def has_text?
    @has_text ||= @content.has_text?
  end

  def write(dir)
    file_path = dir / path
    file_path.parent.mkpath
    file_path.open('w') {|file| file.write to_svg}
  end
end