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
      @controls = spec[:controls]
                      .map {|spec| Panel::control_from(spec: spec, foreground: foreground, background: background)}
    end

    def x(column)
      @columns[column-1]
    end

    def y(row)
      @rows[row-1]
    end

    def to_s
      control_strings = @controls.map(&:to_s).join("\n")
      "#{@name} #{@width} #{@foreground.inspect} #{@background.inspect}\n#{control_strings}"
    end

    def image_svg_file
      path = @base_file_name.sub_ext('.svg')
      xml = Builder::XmlMarkup.new(indent: 2)
      content = xml.svg(version: "1.1", xmlns: "http://www.w3.org/2000/svg",
                        width: @width * PX_PER_MM,
                        height: PANEL_HEIGHT * PX_PER_MM) do |svg|
        svg.g(transform: "scale(#{PX_PER_MM})") do |g|
          g.rect(x: 0, y: 0, width: @width, height: PANEL_HEIGHT, stroke: @foreground, fill: @background, 'stroke-width' => 1)
          g.text(@name.upcase, x: @width / 2, y: PANEL_LABEL_INSET,
                 'dominant-baseline' => 'baseline', 'text-anchor' => 'middle', fill: @foreground,
                 style: "font-family:Proxima Nova;font-weight:bold;font-size:4.064px")
          g.text('DHE', x: @width / 2, y: PANEL_HEIGHT - PANEL_LABEL_INSET,
                 'dominant-baseline' => 'hanging', 'text-anchor' => 'middle', fill: @foreground,
                 style: "font-family:Proxima Nova;font-weight:bold;font-size:#{PANEL_FONT}px")
          @controls.each do |control|
            control.draw_on_image(panel: self, svg: g)
          end
        end
      end
      SvgFile.new(path: path, content: content)
    end

    def image_svg
    end

    def control_svgs
    end

    def self.control_from(spec:, foreground:, background:)
      case spec[:type]
      when 'knob'
        Knob.new(spec: spec, foreground: foreground, background: background)
      when 'button'
        Button.new(spec: spec, foreground: foreground, background: background)
      when 'port'
        Port.new(spec: spec, foreground: foreground, background: background)
      when 'toggle'
        Toggle.new(spec: spec, foreground: foreground, background: background)
      when 'counter'
        Counter.new(spec: spec, foreground: foreground, background: background)
      else
        "Unknown control type #{type}"
      end
    end
  end
end
