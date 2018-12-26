require 'builder'
require 'color'
require 'oj'

require_relative 'control'
require_relative 'dimensions'
require_relative 'svg-file'

module DHE
  JSON_PARSING_OPTIONS = {symbol_keys: true}

  class Module
    attr_reader :name, :foreground, :background

    def initialize(json_file:)
      spec = Oj.load_file(json_file.to_s, JSON_PARSING_OPTIONS)
      @name = spec[:name].upcase
      @module_path = Pathname(@name.downcase.sub(' ', '-'))
      @width = spec[:hp] * MM_PER_HP
      @rows = spec[:rows]
      @columns = spec[:columns]
      @foreground = "##{Color::HSL.new(*spec[:colors][:foreground]).to_rgb.hex}"
      @background = "##{Color::HSL.new(*spec[:colors][:background]).to_rgb.hex}"
      @controls = spec[:controls].map {|spec| control_from(options: spec)}
    end

    def x(column)
      @columns[column - 1]
    end

    def y(row)
      @rows[row - 1]
    end

    def faceplate_svg
      to_svg do |svg, control|
        control.draw_faceplate(svg: svg, x: x(control.column), y: y(control.row))
      end
    end

    def faceplate_file
      SvgFile.new(path: (@module_path / 'faceplate'), content: faceplate_svg, has_text: true)
    end

    def manual_svg
      to_svg do |svg, control|
        control.draw_manual(svg: svg, x: x(control.column), y: y(control.row))
      end
    end

    def manual_image_file
      SvgFile.new(path: @module_path, content: manual_svg, has_text: true)
    end

    def to_svg
      Builder::XmlMarkup.new(indent: 2)
          .svg(version: "1.1", xmlns: "http://www.w3.org/2000/svg",
               width: @width * PX_PER_MM,
               height: PANEL_HEIGHT * PX_PER_MM) do |svg|
        svg.g(transform: "scale(#{PX_PER_MM})") do |g|
          g.rect(x: 0, y: 0, width: @width, height: PANEL_HEIGHT, stroke: @foreground, fill: @background, 'stroke-width' => 1)
          Label.new(text: @name.upcase, size: :panel, color: @foreground, alignment: :above)
              .draw_svg(svg: g, x: @width / 2, y: PANEL_LABEL_INSET)
          Label.new(text: 'DHE', size: :panel, color: @foreground, alignment: :below)
              .draw_svg(svg: g, x: @width / 2, y: PANEL_HEIGHT - PANEL_LABEL_INSET)
          @controls.each do |control|
            yield(g, control)
          end
        end
      end
    end

    def control_files
      @controls.flat_map {|control| control.control_files(module_path: @module_path)}
    end

    def control_from(options:)
      case options[:type]
      when 'knob'
        KnobControl.new(self, options)
      when 'button'
        ButtonControl.new(self, options)
      when 'port'
        PortControl.new(self, options)
      when 'toggle'
        ToggleControl.new(self, options)
      when 'counter'
        CounterControl.new(self, options)
      else
        "Unknown control type #{type}"
      end
    end
  end
end
