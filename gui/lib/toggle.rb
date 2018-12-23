require_relative 'control'

module DHE
  class Toggle < Control
    def initialize(spec:, foreground:, background:)
      super(spec: spec)
      @labels = spec[:labels]
      @position = (spec[:position] || 1) - 1
    end

    def to_s
      "#{super} toggle #{@labels} (#{@labels[@position]})"
    end
  end
end