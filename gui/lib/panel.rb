require 'builder'
require 'color'
require 'oj'

require_relative 'counter.rb'
require_relative 'button'
require_relative 'knob'
require_relative 'port'
require_relative 'toggle'

require_relative 'dimensions'
require_relative 'svg-file'
require_relative 'text'

module DHE
  JSON_PARSING_OPTIONS = {symbol_keys: true}

  class Panel
    attr_reader :name, :foreground, :background

    def initialize(json_file:)
      spec = Oj.load_file(json_file.to_s, JSON_PARSING_OPTIONS)
      @name = spec[:name]
      @base_file_name = Pathname(@name.downcase.sub(' ', '-'))
      @width = spec[:hp] * MM_PER_HP
      @rows = spec[:rows]
      @columns = spec[:columns]
      @foreground = "##{Color::HSL.new(*spec[:colors][:foreground]).to_rgb.hex}"
      @background = "##{Color::HSL.new(*spec[:colors][:background]).to_rgb.hex}"
      @controls = spec[:controls].map {|spec| Panel::control_from(spec: spec)}
    end

    def x(column)
      @columns[column - 1]
    end

    def y(row)
      @rows[row - 1]
    end

    def to_s
      control_strings = @controls.map(&:to_s).join("\n")
      "#{@name} #{@width} #{@foreground.inspect} #{@background.inspect}\n#{control_strings}"
    end

    def image_svg_file(dir:)
      path = dir / @base_file_name.sub_ext('.svg')
      svg_file(path: path) do |svg, control|
        control.draw_image_svg(svg: svg, x: x(control.column), y: y(control.row), foreground: foreground, background: background)
      end
    end

    def panel_svg_file(dir:)
      path = dir / @base_file_name / 'panel.svg'
      svg_file(path: path) do |svg, control|
        control.draw_background_svg(svg: svg, x: x(control.column), y: y(control.row), foreground: foreground, background: background)
      end
    end

    def svg_file(path:)
      xml = Builder::XmlMarkup.new(indent: 2)
      content = xml.svg(version: "1.1", xmlns: "http://www.w3.org/2000/svg",
                        width: @width * PX_PER_MM,
                        height: PANEL_HEIGHT * PX_PER_MM) do |svg|
        svg.g(transform: "scale(#{PX_PER_MM})") do |g|
          g.rect(x: 0, y: 0, width: @width, height: PANEL_HEIGHT, stroke: @foreground, fill: @background, 'stroke-width' => 1)
          Text.new(text: @name.upcase, size: :panel, alignment: :above)
              .draw_svg(svg: g, x: @width / 2, y: PANEL_LABEL_INSET, color: @foreground)
          Text.new(text: 'DHE', size: :panel, alignment: :below)
              .draw_svg(svg: g, x: @width / 2, y: PANEL_HEIGHT - PANEL_LABEL_INSET, color: @foreground)
          @controls.each do |control|
            yield(g, control)
          end
        end
      end
      SvgFile.new(path: path, content: content)
    end

    def control_svg_files(dir:)
    end

    def self.control_from(spec:)
      case spec[:type]
      when 'knob'
        Knob.new(spec: spec)
      when 'button'
        Button.new(spec: spec)
      when 'port'
        Port.new(spec: spec)
      when 'toggle'
        Toggle.new(spec: spec)
      when 'counter'
        Counter.new(spec: spec)
      else
        "Unknown control type #{type}"
      end
    end
  end
end
