require_relative 'control'

module DHE
  class Counter < Control
    def initialize(spec:, foreground:, background:)
      super(spec: spec)
      @labels = spec[:labels]
      @position = (spec[:position] || 1) - 1
      @special = spec[:special]
    end

    def to_s
      "#{super} counter #{@labels} (#{@labels[@position]}) #{@special}"
    end
  end
end