require 'builder'
require 'color'
require 'oj'

require_relative 'dimensions'
require_relative 'button_control'
require_relative 'counter_control'
require_relative 'knob_control'
require_relative 'port_control'
require_relative 'toggle_control'
require_relative 'svg_file'

module DHE
  JSON_PARSING_OPTIONS = { symbol_keys: true }

  class Module
    attr_reader :name, :slug, :foreground, :background

    def initialize(json_file:)
      spec = Oj.load_file(json_file.to_s, JSON_PARSING_OPTIONS)
      @name = spec[:name].upcase
      @slug = Pathname(@name.downcase.sub(' ', '-'))
      @width = spec[:hp] * MM_PER_HP
      @width_px = @width * PX_PER_MM
      @height_px = PANEL_HEIGHT * PX_PER_MM

      @rows = spec[:rows]
      @columns = spec[:columns]
      @foreground = "##{Color::HSL.new(*spec[:colors][:foreground]).to_rgb.hex}"
      @background = "##{Color::HSL.new(*spec[:colors][:background]).to_rgb.hex}"
      @controls = spec[:controls].map { |spec| control_from(options: spec) }
    end

    def x(control:)
      @columns[control.column - 1] + control.x_offset
    end

    def y(control:)
      @rows[control.row - 1] + control.y_offset
    end

    def svg_file(draw_hardware:)
      SvgFile.new(path: slug, width: @width_px, height: @height_px, has_text: true) do |svg|
        svg.g(transform: "scale(#{PX_PER_MM})") do |g|
          g.rect(x: 0, y: 0, width: @width, height: PANEL_HEIGHT,
                 stroke: @foreground, fill: @background, 'stroke-width' => 1)
          Label.new(module_: self, text: @name.upcase, size: :panel)
              .draw_svg(svg: g, x: @width / 2, y: PANEL_LABEL_INSET)
          Label.new(module_: self, text: 'DHE', size: :panel, alignment: :below)
              .draw_svg(svg: g, x: @width / 2, y: PANEL_HEIGHT - PANEL_LABEL_INSET)
          @controls.each do |control|
            x = x(control: control)
            y = y(control: control)
            control.draw_faceplate(svg: g, x: x, y: y)
            control.draw_hardware(svg: g, x: x, y: y) if draw_hardware
          end
        end
      end
    end

    def control_files
      @controls.flat_map(&:control_files)
    end

    def control_from(options:)
      case options[:type]
        when 'button'
          ButtonControl.new(self, options)
        when 'counter'
          CounterControl.new(self, options)
        when 'knob'
          KnobControl.new(self, options)
        when 'port'
          PortControl.new(self, options)
        when 'toggle'
          ToggleControl.new(self, options)
        else
          "Unknown control type #{type}"
      end
    end
  end
end