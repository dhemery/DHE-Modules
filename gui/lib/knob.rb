require_relative 'control'

module DHE
  class Knob < Control
    def initialize(spec:, foreground:, background:)
      super(spec: spec)
      @label = spec[:label]
      @style = spec[:style]
    end

    def to_s
      "#{super}#{' ' + @style if @style} knob (#{@label})"
    end
  end
end