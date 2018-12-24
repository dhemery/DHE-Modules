require_relative 'control'

module DHE
  class Counter < Control
    def initialize(spec:, foreground:, background:)
      super(spec: spec)
      @labels = spec[:labels]
      @position = (spec[:position] || 1) - 1
      @special = spec[:special]
    end

    def draw_on_image(panel:, svg:)
    end

    def draw_on_panel(panel:, svg:)
    end

  end
end