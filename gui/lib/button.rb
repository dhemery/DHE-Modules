require_relative 'control'

module DHE
  class Button < Control
    def initialize(spec:, foreground:, background:)
      super(spec: spec)
      @label = spec[:label]
      @style = spec[:style]
    end

    def to_s
      "#{super} #{@style} button"
    end
  end
end