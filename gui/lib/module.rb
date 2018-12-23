require 'color'
require 'oj'
require_relative 'control'


module DHE
  Oj::default_options = {symbol_keys: true}
  class Module
    MM_PER_INCH = 25.4
    PX_PER_INCH = 75.0
    PX_PER_MM = PX_PER_INCH / MM_PER_INCH
    MM_PER_HP = 5.08

    attr_reader :name, :foreground, :background

    def initialize(name:, width:, foreground:, background:, controls:)
      @name = name
      @width = width * MM_PER_HP
      @foreground = foreground
      @background = background
      @controls = controls.map {|h| Control.from(mod: self, details: h)}
    end

    def to_s
      "#{@name} #{@width} #{@foreground.inspect} #{@background.inspect} #{@controls.map(&:to_s)}"
    end

    def self.from_file(path)
      from(Oj.load_file(path.to_s))
    end

    def self.from(details)
      name = details[:name]
      width = details[:width]
      foreground = Color::HSL.new(*details[:colors][:foreground])
      background = Color::HSL.new(*details[:colors][:background])
      Module.new(name: name, width: width, foreground: foreground, background: background, controls: details[:controls])
    end
  end
end
