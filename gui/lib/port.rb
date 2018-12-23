require_relative 'control'

module DHE
  class Port < Control
    def initialize(spec:, foreground:, background:)
      super(spec: spec)
      @label = spec[:label]
      @style = spec[:style]
      @special = spec[:special]
    end

    def to_s
      "#{super} #{@style} port #{@special}"
    end
  end
end