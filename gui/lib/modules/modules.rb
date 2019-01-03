require_relative 'booster_stage'
require_relative 'cubic'

module DHE
  class Modules
    def self.all
      [
          BoosterStage.new,
          Cubic.new,
      ]
    end
  end
end
