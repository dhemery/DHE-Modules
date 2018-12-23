require 'color'
require 'oj'
require_relative 'button.rb'
require_relative 'counter.rb'
require_relative 'knob.rb'
require_relative 'port.rb'
require_relative 'toggle.rb'

module DHE
  JSON_PARSING_OPTIONS = {symbol_keys: true}

  class Panel
    MM_PER_INCH = 25.4
    PX_PER_INCH = 75.0
    PX_PER_MM = PX_PER_INCH / MM_PER_INCH
    MM_PER_HP = 5.08

    attr_reader :name, :foreground, :background

    def initialize(json_file:)
      spec = Oj.load_file(json_file.to_s, JSON_PARSING_OPTIONS)
      @name = spec[:name]
      @width = spec[:width] * MM_PER_HP
      @foreground = Color::HSL.new(*spec[:colors][:foreground])
      @background = Color::HSL.new(*spec[:colors][:background])
      @controls = spec[:controls]
                      .map {|spec| Panel::control_from(spec: spec, foreground: foreground, background: background)}
    end

    def to_s
      control_strings = @controls.map(&:to_s).join("\n")
      "#{@name} #{@width} #{@foreground.inspect} #{@background.inspect}\n#{control_strings}"
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
